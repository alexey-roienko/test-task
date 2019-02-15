/*! \file config.h
    \brief Config file for 'findContigReg' utility

    File contains the default parameters for utility running and constants for algorithm operation.
*/

#pragma once

/*! \namespace DefParams for distinguishing parameter names for sudden coincidence with parameters in other libraries
*/
namespace DefParams {
	/*! \param DEBUG Turn on/off debug info from code */
	constexpr bool DEBUG = false;
	/*! \param DEFAULT_FNAME Default test image name */
	constexpr const char* DEFAULT_FNAME = "test_colored_pils.png";
	/*! \param DEFAULT_CLDIST_TYPE Default type of color distance metric 
		0 - Simple RGB Euclidian distance
		1 - Simple RGB Euclidian distance with weights (2, 4, 3 for RGB)
		2 - Advanced RGB Euclidian distance with weights (2, 4, 3 for RGB) and additional coefficient
		4 - Simple LAB distance, CIE76
		5 - Advanced LAB distance, CIE94
		All distance metrics are taken from Wiki - https://en.wikipedia.org/wiki/Color_difference
	*/
	constexpr uint8_t DEFAULT_CLDIST_TYPE = 4;
	/*! \param DEFAULT_CLDIST_THRD Default value of color distance metric threshold
		for type 0 try: 40
		for type 1 try:
		for type 2 try: 4000
		for type 3 try: 20
		for type 4 try: 120
		Note that THRD values are squared comparing to the formulas in Wiki, for calculation simplicity.
	*/
	constexpr double DEFAULT_CLDIST_THRD = 120;
	
	/*! \param k_L Parameter value for type 4 distance, CIE94 */
	constexpr int k_L = 1/1;
	/*! \param K1 Parameter value for type 4 distance, CIE94 */
	constexpr double K1 = 0.045;
	/*! \param K2 Parameter value for type 4 distance, CIE94 */
	constexpr double K2 = 0.015;
	/*! \param SL Parameter value for type 4 distance, CIE94 */
	constexpr double SL = 1/1;			
	/*! \param FR256 Parameter value for type 4 distance, CIE94 */
	constexpr double FR256 = 1/256;
}