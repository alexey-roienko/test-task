// Test task for Svitla - findContiguousRegion utility for colored images
// Created by Alexey Roienko, Feb. 2019

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <queue>

using namespace std;
using namespace cv;

void mouseCallback(int event, int x, int y, int flag, void *param);
Mat findRegion(const Mat &, const Point &, bool);


int main(int argc, char* argv[])
{
	cout << "\nUtility allows searching for Contiguous Region on input image.\n" << 
		    "Syntacsis: findContiguousRegion\n" << 
			"           findContiguousRegion <image_name>\n" << 
			"Commands:  press LEFT mouse button on the INPUT IMAGE window for choosing the initial pixel\n" <<
			"           press 'q' for exit the program" << endl;
	
	//Point curPoint(100, 100);
	
	string imFileName = "test_binary.jpg";
	bool debug = false;
	if (argc == 2) {
		imFileName = string(argv[1]);
	}
	if (argc > 2) {
		cout << "Program can have NO (default image wiil be used) or ONE input parameter!\n" << endl;
		return -1;
	} 
	
	Mat img = imread("../input/" + imFileName, IMREAD_COLOR);
	cout << endl << " Image: \"" << imFileName << "\",\n Image size = " << img.size() <<
		"\n Image type = " << img.type() << "\n Channels N = " << img.channels() << endl;
	
	if (img.empty()) {
		cout << "Couldn't open the image" << endl;
		return -1;
	}
	
	namedWindow("Input image", WINDOW_NORMAL | WINDOW_KEEPRATIO);
	setMouseCallback("Input image", mouseCallback, (void*)(&img));
	//cout << "Received position: [" << newPoint.x << ", " << newPoint.y << "]" << endl;
	imshow("Input image", img);

	namedWindow("Output mask", WINDOW_NORMAL | WINDOW_KEEPRATIO);
		
	// Processing...
	string userCommand;
	for(;;){
		int c = waitKey(0);
		if((char)c == 'q') {
			cout << " Exiting ...\n";
			break;
		}
	}
	return 0;
}


void mouseCallback(int event, int x, int y, int flag, void *param){
	if (event == EVENT_LBUTTONDOWN) {
		Mat img(*((Mat*)param));
		Point curPoint(x, y);
		cout << "Captured position: [x = " << x << ", y = " << y << "]" << endl;
		
		// Processing...
		double testTime = (double)getTickCount();
		
		Mat maskImg = findRegion(img, curPoint, false);
		imshow("Output mask", maskImg);
			
		testTime = ((double)getTickCount() - testTime)/getTickFrequency() * 1000;		
		
		cout << " Test time in milliseconds: " << setprecision(4) << testTime << " msec." << endl;
	}
}



Mat findRegion(const Mat & img, const Point & stPixel, bool debug) {
	CV_Assert(img.depth() == CV_8UC1);
	
	cout << img << endl;
	
	// Define chosen pixel value
	int stPixVal = (int)img.at<uchar>(stPixel.y, stPixel.x);
	// Define borders of pixel value for checking procedure
	int lTH = 0, uTH = 255;
	if (stPixVal > 0) (lTH = stPixVal - 15);
	if (stPixVal < 255) (uTH = stPixVal + 15);
	
	//if (debug)
		cout << "Defined pixel coords: [row = " << stPixel.y << "; col = " << stPixel.x <<
			"], Pixel Value = " << stPixVal << 
			", Limits = [" << lTH << ", " << uTH << "]" << endl;
	
	// Create mask image and fill it with 0 - black
	Mat maskImg = Mat::zeros(img.size(), CV_8UC1);
	maskImg.at<uchar>(stPixel.y, stPixel.x) = 255;
	
	// Lambda-function for checking the position coorectness of the current pixel
	auto lPxPosCheck = [&img, debug](int pxRow, int pxCol)->bool{		
		if ((pxCol >= 0 && pxCol <= img.cols-1) &&
			(pxRow >= 0 && pxRow <= img.rows-1)){
			if (debug) cout << "\t POSITION Check - PASSED" << endl;
			return true;
		}
		else {
			if (debug) cout << "\t POSITION Check - FAILED" << endl;
			return false;
		}
	};
	
	// Lambda-function for checking the value of the current pixel on the mask:
	//    have we already checked it or not?
	auto lPxMaskCheck = [&maskImg, debug](int pxRow, int pxCol)->bool{
		uchar pxVal = maskImg.at<uchar>(pxRow, pxCol);
		if (pxVal == 0) {
			if (debug) cout << "\t MASK Check - NOT VISITED YET" << endl;
			return true;
		}
		else {
			if (debug) cout << "\t MASK Check - ALREADY MARKED" << endl;
		}
		return false;
	};
	
	// Lambda-function for checking the value of the current GRAYSCALE pixel
	auto lPxValCheckGray = [&img, &maskImg, lTH, uTH, debug](int pxRow, int pxCol)->bool{
		uchar pxVal = img.at<uchar>(pxRow, pxCol);
		if (pxVal >= lTH && pxVal <= uTH) {
			maskImg.at<uchar>(pxRow, pxCol) = 255;
			if (debug) cout << "\t VALUE Check - PASSED" << endl;
			return true;
		}
		else {
			maskImg.at<uchar>(pxRow, pxCol) = 100;
			if (debug) cout << "\t VALUE Check - FAILED" << endl;
		}
		return false;
	};
	
	// Lambda-function for checking the value of the current COLOR pixel
	auto lPxValCheckColor = [&img, &maskImg, lTH, uTH, debug](int pxRow, int pxCol)->bool{
		Vec3b pxVal = img.at<Vec3b>(pxRow, pxCol);
				
		/*if (pxVal >= lTH && pxVal <= uTH) {
			maskImg.at<uchar>(pxRow, pxCol) = 255;
			if (debug) cout << "\t VALUE Check - PASSED" << endl;
			return true;
		}
		else {
			maskImg.at<uchar>(pxRow, pxCol) = 100;
			if (debug) cout << "\t VALUE Check - FAILED" << endl;
		}*/
		return false;
	};
		
	queue<Point> fifo;
	
	// Push the initial pixels located on cross with the center in stPixel
	int X = stPixel.x, Y = stPixel.y-1;
	if (debug) cout << "pxRow = " << (int)Y << ", pxCol = " << (int)X << ':' << endl; 
	if (lPxPosCheck(Y, X) && lPxValCheckGray(Y, X))
		fifo.push(Point(X, Y));
	
	X = stPixel.x+1, Y = stPixel.y;
	if (debug) cout << "pxRow = " << (int)Y << ", pxCol = " << (int)X << ':' << endl; 
	if (lPxPosCheck(Y, X) && lPxValCheckGray(Y, X))
		fifo.push(Point(X, Y));
	
	X = stPixel.x, Y = stPixel.y+1;
	if (debug) cout << "pxRow = " << (int)Y << ", pxCol = " << (int)X << ':' << endl; 
	if (lPxPosCheck(Y, X) && lPxValCheckGray(Y, X))
		fifo.push(Point(X, Y));
	
	X = stPixel.x-1, Y = stPixel.y;
	if (debug) cout << "pxRow = " << (int)Y << ", pxCol = " << (int)X << ':' << endl; 
	if (lPxPosCheck(Y, X) && lPxValCheckGray(Y, X))
		fifo.push(Point(X, Y));
	
	
	while (not fifo.empty())
	{
		Point px2Check = fifo.front();
		
		// Checking the first neighbour pixel
		int X = px2Check.x, Y = px2Check.y-1;
		if (debug) cout << "pxRow = " << (int)Y << ", pxCol = " << (int)X << ':' << endl; 
		if (lPxPosCheck(Y, X) && lPxMaskCheck(Y, X) && lPxValCheckGray(Y, X))
			fifo.push(Point(X, Y));	
		
		// Checking the second neighbour pixel
		X = px2Check.x+1, Y = px2Check.y;
		if (debug) cout << "pxRow = " << (int)Y << ", pxCol = " << (int)X << ':' << endl; 
		if (lPxPosCheck(Y, X) && lPxMaskCheck(Y, X) && lPxValCheckGray(Y, X))
			fifo.push(Point(X, Y));	
		
		// Checking the third neighbour pixel
		X = px2Check.x, Y = px2Check.y+1;
		if (debug) cout << "pxRow = " << (int)Y << ", pxCol = " << (int)X << ':' << endl; 
		if (lPxPosCheck(Y, X) && lPxMaskCheck(Y, X) && lPxValCheckGray(Y, X))
			fifo.push(Point(X, Y));	
		
		// Checking the fourth neighbour pixel
		X = px2Check.x-1, Y = px2Check.y;
		if (debug) cout << "pxRow = " << (int)Y << ", pxCol = " << (int)X << ':' << endl; 
		if (lPxPosCheck(Y, X) && lPxMaskCheck(Y, X) && lPxValCheckGray(Y, X))
			fifo.push(Point(X, Y));
		
		fifo.pop();
	}
	
	return maskImg;
}


















