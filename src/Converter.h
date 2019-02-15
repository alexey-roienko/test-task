#pragma once

#include <string>
#include <queue>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "../src/config.h"

/*! \class Converter
    \brief Header of class Converter

    Class calculates the contiguous region and converts it to the binary (mask) image.
*/
class Converter {
	
	private:
		//! Private variable, contains input test image
		cv::Mat baseImg;
		//! Private variable, contains obtained mask image
		cv::Mat maskImg;
		//! Private variable, contains position of pixel selected by user
		cv::Point pxPos;
		//! Private variable, contains the color values of pixel selected by user
		cv::Vec3b pxVal;
		//! Private variable, whether output debug info or not
		bool debug = DefParams::DEBUG;
		//! Private variable, contains inputed type of the color distance to be applied
		uint8_t clDistType;
		//! Private variable, contains inputed threshold value of the chosen color distance
		double clDistThr;
		
		//! Check the position of the pixel at hand; whether it is inside of image borders or not
		bool checkPxPos(int pxRow, int pxCol);
		//! Check the value of the current pixel on the mask - have we already checked it or not
		bool checkPxMask(int pxRow, int pxCol);
		//! Check the color value of the current pixel
		bool checkPxColor(int pxRow, int pxCol);
		//! Main method which calculates the distance between pixels
		double calcPixelDistance(const cv::Vec3b & px2Comp, const cv::Vec3b & initPx, uint8_t distType);
		
	
	public:
		//! Default constructor
		Converter() = default;
	
		//! Main constructor
		Converter(const cv::Mat &, bool);
		
		//! Setter for parameters for pixel comparing
		void setParams(uint8_t, double);
		
		//! Setter for Debug
		void setDebug(bool);
		
		//! Setter for base Image
		void setBaseImg(const cv::Mat &);
		
		//! Getter - Returns 'maskImg' content
		cv::Mat getMaskImg();
		
		//! Shows Image in Window
		void showMaskImg();
		
		//! Reset Mask image to initial state
		void resetMaskImg();
		
		//! Set Pixel point to be processed by main function
		void setPoint(const cv::Point &);
				
		//! Main function for finding the contiguous region
		bool findRegion();
		
};




