/*! \file main.cpp
    \brief The starting point of the utility.

	Utility allows searching for contiguous region on input image.
    Utility has three non-obligatory parameters:
		1. <test_image> - images should be located in '\input'-folder
		2. <cl_dist_type> - int, [0; 4], the type of color distance which will be applied by algorithm
		3. <cl_dist_thld> - double, >0, the threshold value for defining the pixels similarity
	
	Syntacsis: 
		findContigReg 
		findContigReg <test_image>
		findContigReg <test_image> <cl_dist_type>
		findContigReg <test_image> <cl_dist_type> <cl_dist_thld>
		
		(for default values see \file config.h)
		
	Commands:  
		- press LEFT mouse button on the INPUT IMAGE window for choosing the initial pixel;
		- press 'q' for exit the program (should be pressed while the 'Base image' window is active).
*/

#include "src/Visualizer.h"
#include "src/config.h"
#include <iostream>


void help();


/*! Main function for running the utility.
 *  @param argc, argv[] The number of inputed parameters and their values.
 */
int main(int argc, char* argv[])
{
	using namespace std;
	using namespace cv;

	// Output brief notes about utility
	help();	
	
	// Set default base image name as working one
	string baseImgFName(DefParams::DEFAULT_FNAME);
	// Default color distance type
	uint8_t clDistType = DefParams::DEFAULT_CLDIST_TYPE;
	// Default color distance threshold value
	double clDistThr = DefParams::DEFAULT_CLDIST_THRD;
	
	// Check the number of utility input arguments
	switch (argc){
		// Nothing is set up
		case 1: {
			break;
		}
		// Base image name is set only
		case 2: {
			baseImgFName.assign(argv[1]);
			break;
		}
		// Base image name and Color distance type are set up only
		case 3: {
			baseImgFName.assign(argv[1]);
			clDistType = stoi(argv[2]);
			break;
		}
		// Base image name, Color distance type and its Threshold are set up
		case 4: {
			baseImgFName.assign(argv[1]);
			clDistType = stoi(argv[2]);
			clDistThr = stod(argv[3]);
			break;
		}
		// if (argc > 4)...
		default:{
			cout << " Too many parameters!\n See brief overview for details...\n" << endl;
			return -1;
		}
	}
	
	// Initialize main object with the name of inputed image
	Visualizer vizObj(baseImgFName);
	// Read the test image
	vizObj.readBaseImg();
	// Show the test image in the separate window using openCV-function
	vizObj.showBaseImg();
	// Main method for starting the processing of the image by running the mouse callback
	vizObj.startProcessing(clDistType, clDistThr);
	// Output of the utility for further processing
	Mat binaryMask = vizObj.getMaskImg();
			
	// The part of the code to process the user input for quiting the utility
	for(;;){
		int c = waitKey(0);
		if((char)c == 'q') {
			cout << " Exiting ...\n";
			break;
		}
	}
	return 0;
}


/*! Function outputs the brief info about the utility and its parameters.
 */
void help() {
	std::cout << "\nUtility allows searching for Contiguous Region on input image.\n" << 
		    "Syntacsis:\n" 
			"           findContigReg\n" << 
			"           findContigReg <test_image>\n" << 
			"           findContigReg <test_image> <cl_dist_type>\n" <<
			"           findContigReg <test_image> <cl_dist_type> <cl_dist_thld>\n" <<
			"Commands:  \n"
			"           press LEFT mouse button on the INPUT IMAGE window for choosing the initial pixel\n" <<
			"           press 'q' for exit the program (should be pressed while the 'Base image' window is active)" << 
			std::endl;
}















