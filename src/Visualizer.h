/*! \headerfile Visualizer.h "/scr/Visualizer.h"
    \brief Displays the test image as well as calculated image binary mask

    Class displays the test image as well as calculated image binary mask
*/

#pragma once

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "../src/Converter.h"


class Visualizer {
	
	private:
		//! Private variable, contains input test image filename
		std::string fName;
		//! Private variable, contains input test image
		cv::Mat baseImg;
		//! Private variable, contains Converter object
		Converter convObj;
	
	public:		
		//! Constructor of the class
		/*! 
		  \param _fName - filename of the input test color image 
		*/
		Visualizer(const std::string & _fName);
		
		//! Methods reads test (base) image using its filename and shows it in window
		bool readBaseImg();
		
		//! Getter for test (base) image
		cv::Mat getBaseImg();
		
		//! Visualize the test (base) image in a window
		bool showBaseImg();
		
		//! Getter for mask (binary, output) image
		cv::Mat getMaskImg();
		
		//! Visualize the mask (binary, output) image in a window
		bool showMaskImg();
		
		//! Main function of the utility
		/*!
			\param clDistType - type of the color distance to be applied
			\param clDistThr  - threshold value for the color distance to be applied
		*/
		bool startProcessing(uint8_t, double);
		
		//! One of the main functions of the utility
		/*!
			\param event - the type of the mouse activity, int
			\param x  - X coordinate of mouse position in the window while clicking
			\param y  - Y coordinate of mouse position in the window while clicking
			\param flag  - flag parameter (didn't use it)
			\param param  - pointer for transferring user data if necessary 
						   (used it to send the pointer to the Visualizer-object)
		*/
		static void mCallback_Func(int, int, int, int, void *);
		
		//! One of the main functions of the utility
		/*!
			\param x  - X coordinate of mouse position in the window while clicking
			\param y  - Y coordinate of mouse position in the window while clicking
		*/
		void mCallback(int, int);
		
};







