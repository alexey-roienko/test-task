/*! \class Converter
    \brief Class calculates the contiguous region and converts it to the binary (mask) image.

    Class calculates the contiguous region and converts it to the binary (mask) image.
*/

#include <iostream>
#include <string>
#include "../src/Converter.h"
#include "../src/config.h"


//! Constructor of the class
/*! 
  \param _img - input test color image
  \param _debug - debug information switcher
*/
Converter::Converter(const cv::Mat & _img, bool _debug = false):
	baseImg(_img), debug(_debug)
{
	maskImg = cv::Mat::zeros(baseImg.size(), CV_8UC1);
};

//! Setter for 'debug' parameter
void Converter::setDebug(bool _debug) {
	debug = _debug;
}


//! Setter for parameters for pixel comparing
void Converter::setParams(uint8_t _clDistType, double _clDistThr) {
	clDistType = _clDistType;
	clDistThr = _clDistThr;
}
		

//! Getter - Returns 'maskImg' content
void Converter::setBaseImg(const cv::Mat & _baseImg) {	
	baseImg = _baseImg;
	maskImg = cv::Mat::zeros(baseImg.size(), CV_8UC1);
}


//! Getter - Returns 'maskImg' content
cv::Mat Converter::getMaskImg() {	
	return maskImg;
}


//! Reset Mask image to initial state
void Converter::resetMaskImg(){
	maskImg = cv::Mat::zeros(baseImg.size(), CV_8UC1);
}


//! Shows Image in Window
void Converter::showMaskImg() {
	using namespace cv;	
	// Show the window for the mask image on the screen
	namedWindow("Output mask", WINDOW_NORMAL | WINDOW_KEEPRATIO);
	// Show obtained mask image in the corresponding window
	imshow("Output mask", maskImg);	
}


//! Set Pixel point to be processed by main function
void Converter::setPoint(const cv::Point & _pxPos) {
	pxPos = _pxPos;
	pxVal = baseImg.at<cv::Vec3b>(pxPos.y, pxPos.x);
	if (debug)
		std::cout << "Defined pixel coords: [row = " << pxPos.y << "; col = " << pxPos.x <<
			"], Pixel Value = " << pxVal << std::endl;
}	


//! FindRegion function - the main one
bool Converter::findRegion() {
	using namespace cv;	
	using namespace std;	
	
	// The first pixel is always TRUE and is painted by white color
	maskImg.at<uchar>(pxPos.y, pxPos.x) = 255;
	// Queue for storing the pixels to be checked
	queue<Point> fifo;
	
	// Lambda-function for checking the current pixel
	auto lPxCheck = [&fifo, this](int pxX, int pxY)->bool{
		//if (this->debug)
			//cout << "lPxCheck:  pxRow = " << pxY << ", pxCol = " << pxX <<  endl; 
		// Perform checking
		if (checkPxPos(pxY, pxX) && checkPxMask(pxY, pxX) && checkPxColor(pxY, pxX)) {
			fifo.push(Point(pxX, pxY));
			return true;
		}
		return false;
	};
		
	// Push the initial pixels located on cross with the center in 'pixel'
	int X = pxPos.x;
	int Y = pxPos.y-1;
	lPxCheck(X, Y);
	X = pxPos.x+1;       Y = pxPos.y;      lPxCheck(X, Y);	
	X = pxPos.x;         Y = pxPos.y+1;	   lPxCheck(X, Y);
	X = pxPos.x-1;       Y = pxPos.y;      lPxCheck(X, Y);
	
	while (not fifo.empty()) {
		Point px2Check = fifo.front();		
		// Checking the first neighbour pixel
		X = px2Check.x;   Y = px2Check.y-1;  lPxCheck(X, Y);		
		// Checking the second neighbour pixel
		X = px2Check.x+1; Y = px2Check.y;    lPxCheck(X, Y);		
		// Checking the third neighbour pixel
		X = px2Check.x;   Y = px2Check.y+1;  lPxCheck(X, Y);		
		// Checking the fourth neighbour pixel
		X = px2Check.x-1; Y = px2Check.y;    lPxCheck(X, Y);
		// Remove the checked point
		fifo.pop();
	}
	return 0;
}


//! Check the position of the pixel at hand; whether it is inside of image borders or not
bool Converter::checkPxPos(int pxRow, int pxCol) {
	using namespace std;
	if ((pxCol >= 0 && pxCol <= baseImg.cols-1) &&
		(pxRow >= 0 && pxRow <= baseImg.rows-1)){
		if (debug) cout << "\t POSITION Check - PASSED" << endl;
		return true;
	}
	else {
		if (debug) cout << "\t POSITION Check - FAILED" << endl;
		return false;
	}
};


//! Check the value of the current pixel on the mask - have we already checked it or not
bool Converter::checkPxMask(int pxRow, int pxCol) {
	using namespace std;
	
	uchar pxMaskVal = maskImg.at<uchar>(pxRow, pxCol);
	if (pxMaskVal == 0) {
		if (debug) cout << "\t MASK Check - NOT VISITED YET, " << (int)pxMaskVal << endl;
		return true;
	}
	else {
		if (debug) cout << "\t MASK Check - ALREADY MARKED" << endl;
	}
	return false;
};


//! Check the color value of the current pixel
bool Converter::checkPxColor(int pxRow, int pxCol) {
	using namespace std;
	using namespace cv;
	
	Vec3b newPxVal = baseImg.at<Vec3b>(pxRow, pxCol);
	
	//cout << "checkPxColor: newPxVal = " << newPxVal << ", pxVal = " << pxVal << endl;
	double pxDist = calcPixelDistance(newPxVal, pxVal, clDistType);
	if (pxDist <= clDistThr) {			
		maskImg.at<uchar>(pxRow, pxCol) = 255;
		if (debug) cout << "\t VALUE Check - PASSED" << endl;
		return true;
	}
	else {
		//cout << "Color distance = " << pxDist << ", greater than TH" << endl;
		maskImg.at<uchar>(pxRow, pxCol) = 100;
		if (debug) cout << "\t VALUE Check - FAILED" << endl;
	}
	return false;
};

//! Main method which calculates the distance between pixels
double Converter::calcPixelDistance(const cv::Vec3b & px2Comp, const cv::Vec3b & initPx, uint8_t distType) {
	using namespace std;
	
	double dist = 0;
	switch (distType) {
		// Simple BRG distance
		case 0: {
			dist = (px2Comp[2]-initPx[2])*(px2Comp[2]-initPx[2]) + 
			       (px2Comp[1]-initPx[1])*(px2Comp[1]-initPx[1]) + 
				   (px2Comp[0]-initPx[0])*(px2Comp[0]-initPx[0]);
			break;
		}
		// Simple BRG distance with coefficients
		case 1: {
			dist = 2*(px2Comp[2]-initPx[2])*(px2Comp[2]-initPx[2]) + 
			       4*(px2Comp[1]-initPx[1])*(px2Comp[1]-initPx[1]) + 
				   3*(px2Comp[0]-initPx[0])*(px2Comp[0]-initPx[0]);
			break;
		}
		// Advanced BRG distance
		case 2: {
			double dR2 = (px2Comp[2]-initPx[2]) * (px2Comp[2]-initPx[2]);
			double midR = (px2Comp[2] + initPx[2])*0.5;
			double dG2 = (px2Comp[1]-initPx[1]) * (px2Comp[1]-initPx[1]);
			double dB2 = (px2Comp[0]-initPx[0]) * (px2Comp[0]-initPx[0]);
			dist = 2*dR2 + 4*dG2 + 3*dB2 + midR * (dR2 - dB2) * DefParams::FR256;
			break;
		}
		// Simple Lab distance - CIE 76
		case 3: {
			double dL2 = (px2Comp[0]-initPx[0]) * (px2Comp[0]-initPx[0]);
			double dA2 = (px2Comp[1]-initPx[1]) * (px2Comp[1]-initPx[1]);
			double dB2 = (px2Comp[2]-initPx[2]) * (px2Comp[2]-initPx[2]);
			dist = dL2 + dA2 + dB2;
			break;
		}
		// Advanced Lab distance - CIE 94
		case 4: {
			double dL2 = (px2Comp[0] - initPx[0]) * (px2Comp[0] - initPx[0]);
			double C1_2 = px2Comp[1] * px2Comp[1] + px2Comp[2] * px2Comp[2];
			double C2_2 = initPx[1] * initPx[1] + initPx[2] * initPx[2];
			double C1 = exp(0.5 * log(C1_2));
			double C2 = exp(0.5 * log(C2_2));
			double dC2 = (C1 - C2) * (C1 - C2);
			double dA2 = (px2Comp[1] - initPx[1]) * (px2Comp[1] - initPx[1]);
			double dB2 = (px2Comp[2] - initPx[2]) * (px2Comp[2] - initPx[2]);
			double dH2 = dA2 + dB2 - dC2;
			double SC = 1 + DefParams::K1 * C1;
			double SH = 1 + DefParams::K2 * C1;
			if ((DefParams::k_L - 1 < 1e-5) && (DefParams::SL - 1 < 1e-5))
				dist = dL2 + dC2/(SC*SC) + dH2/(SH*SH);
			else if (DefParams::k_L - 1 < 1e-5)
				dist = dL2/(DefParams::SL*DefParams::SL) + dC2/(SC*SC) + dH2/(SH*SH);
			else if (DefParams::SL - 1 < 1e-5)
				dist = dL2/(DefParams::k_L*DefParams::k_L) + dC2/(SC*SC) + dH2/(SH*SH);
			else
				dist = dL2/(DefParams::k_L*DefParams::k_L)/(DefParams::SL*DefParams::SL) + dC2/(SC*SC) + dH2/(SH*SH);
			break;
		}
	}
	if (debug) cout << "\t Color distance = " << setprecision(5) << dist << endl;
	return dist;
}










