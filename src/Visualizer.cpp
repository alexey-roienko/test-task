/*! \class Visualizer
    \brief Displays the test image as well as calculated image binary mask

    Class displays the test image as well as calculated image binary mask
*/

#include <iostream>
#include <string>
#include "../src/Visualizer.h"


//! Constructor of the class
/*! 
  \param _fName - filename of the input test color image 
*/
Visualizer::Visualizer(const std::string & _fName):
	fName(_fName)
{};


//! Methods reads test (base) image using its filename and shows it in window
bool Visualizer::readBaseImg() {
	using namespace std;
	using namespace cv;
	
	baseImg = imread("../input/" + fName, IMREAD_COLOR);
	
	cout << endl << " Image: \"" << fName <<
					"\",\n Image size = " << baseImg.size() <<
					"\n Image type = " << baseImg.type() << 
					"\n Channels N = " << baseImg.channels() << endl;
	
	if (baseImg.empty()) {
		cout << "Couldn't open the image" << endl;
		return -1;
	}	
	return 0;
}


//! Getter for test (base) image
cv::Mat Visualizer::getBaseImg(){
	return baseImg;
};


//! Visualize the test (base) image in a window
bool Visualizer::showBaseImg() {
	using namespace cv;
	
	// Show the window for the base image on the screen
	namedWindow("Base image", WINDOW_NORMAL | WINDOW_KEEPRATIO);
	// Show the downloaded base image in the corresponding window
	imshow("Base image", baseImg);	
	return 0;
}


//! Getter for mask (binary, output) image
cv::Mat Visualizer::getMaskImg(){
	return convObj.getMaskImg();
};


//! Visualize the mask (binary, output) image in a window
bool Visualizer::showMaskImg() {
	convObj.showMaskImg();
	return 0;
}


//! Main function of the utility
/*!
	\param clDistType - type of the color distance to be applied
	\param clDistThr  - threshold value for the color distance to be applied
*/
bool Visualizer::startProcessing(uint8_t clDistType, double clDistThr) {
	using namespace cv;
	// Send test (base) image to the object
	convObj.setBaseImg(baseImg);
	// Send necessary parameters to the object
	convObj.setParams(clDistType, clDistThr);
	// Set mouse callback for choosing the pixel by mouse pointer in the window with test (base) image
	setMouseCallback("Base image", mCallback_Func, this);
	return 0;
}

//! One of the main functions of the utility
/*!
	\param event - the type of the mouse activity, int
	\param x  - X coordinate of mouse position in the window while clicking
	\param y  - Y coordinate of mouse position in the window while clicking
	\param flag  - flag parameter (didn't use it)
	\param param  - pointer for transferring user data if necessary 
	               (used it to send the pointer to the Visualizer-object)
*/
void Visualizer::mCallback_Func(int event, int x, int y, int flag, void *param){
	// If the user pressed the LEFT button...
	if (event == cv::EVENT_LBUTTONDOWN) {		
		// Recover current object
		Visualizer* obj = (Visualizer*)param;
		// Call the Callback method owned by the Visualizer object with mouse pointer coordinates
		obj->Visualizer::mCallback(x, y);
	}
}


//! One of the main functions of the utility
/*!
	\param x  - X coordinate of mouse position in the window while clicking
	\param y  - Y coordinate of mouse position in the window while clicking
*/
void Visualizer::mCallback(int x, int y){
	using namespace std;
	using namespace cv;
	
	// Output debug info
	if (DefParams::DEBUG) cout << "Captured position: [x = " << x << ", y = " << y << "]" << endl;
	// Set up chosen pixel coordinates on the test (base image)
	convObj.setPoint(Point(x, y));
	// Reset binary mask for removing any previous results
	convObj.resetMaskImg();
	// Fix the time before running the algorithm
	double testTime = (double)getTickCount();
	// Run main algorithm
	convObj.findRegion();
	// Fix the time when algorithms finishes
	testTime = ((double)getTickCount() - testTime)/getTickFrequency() * 1000;
	cout << " Test time in milliseconds: " << setprecision(4) << testTime << " msec." << endl;
	
	// Show the obtained binary mask on separate window
	convObj.showMaskImg();	
}






