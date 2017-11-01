/*
File Created on 1-Nov-2017

CHANGES:

Name: Hans
Date: 1-Nov-2017
Time: 1000 - ...
Description:		1) File created
					2) Skeleton created
					3) Note: add backdoor method of exiting main
					4) Note: define temperature threshold as pre-processor directive OR make it dynamic
					5) Note: download provided links as PDFs

*/

/*
Lab objectives from provided materials:
	1) program I2C devices from Linux. Use Linux I2C libraries and APIs
	2) programming on Linux to handle webcam and capture images. Store images on SD card.
	3) Use temperature sensor to trigger capture of images from webcam.
*/


int main() {

	/*declare and initialise variables here*/

	/*PART 1 - COMPLETE FIRST OBJECTIVE*/

	// set up I2C protocol on devices (program the devices?)
	/*Devices:
		PIC? probably not
		TMP102 (temperature sensor)
		USB webcam
	*/
	// I2C on A4 (SDA) and A5 (SCL) of galileo

	/*END PART 1*/

	// FIND PROTOCOL TO MAKE TEMPERATURE DYNAMIC

	/*PART 2 - COMPLETE SECOND AND THIRED OBJECTIVES*/

	// infinite loop - maybe add backdoor method of exiting
	while (1) {

		// communicate via I2C with temperature sensor
		// get temperature
		// end I2C communicay

		if (/*temerapture > threshold*/) {
			
			// start I2C communicay with USB webcam
			// capture image and store it on filesystem
				// What I think protocol is
				// send capture command via I2C
				// create appropriate file on filesystem
				// transfer data via I2C and put in buffer or directly to file
			// end I2C communicay

			/*
			YOU DON'T WANT TO MANY IMAGE CAPTURES AT ONCE, SO PUT A SLEEP HERE, I THINK
			You want it in if-statement here for fast polling of temperature, but once image is captured, delay possibility of next image
			*/

		}


	}

	/*END PART 2*/

	return 0;
} // end main