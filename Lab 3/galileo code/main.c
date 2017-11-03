/*
File Created on 1-Nov-2017

CHANGES:

Name: Hans
Date: 1-Nov-2017
Time: 1000 - 1044
Description:		1) File created
					2) Skeleton created
					3) Note: add backdoor method of exiting main
					4) Note: define temperature threshold as pre-processor directive OR make it dynamic
					5) Note: download provided links as PDFs

Name: Hans
Date: 3-Nov-2017
Time: 1015 - ...
Description:
				1) We ran "i2cdetect -l" and saw i2c-0
				2) We rand "i2cdetect -r 0" and got a table with something showing up at address 0x48 (the TMP102).  So it is connected via I2C.

*/

/*
Lab objectives from provided materials:
	1) program I2C devices from Linux. Use Linux I2C libraries and APIs
	2) programming on Linux to handle webcam and capture images. Store images on SD card.
	3) Use temperature sensor to trigger capture of images from webcam.
*/

#include <linux/i2c-dev.h>		// access i2c adapter from linux program; this may be incorrect library
#define ADAPTER_NUMBER 0		// is determined dynamically [inspect /sys/class/i2c-dev/ or run "i2cdetect -l" to decide this.]

/*STUFF I STOLE FROM INTERNET*/
#include <glib.h>
#include <glib/gprintf.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/*END*/

int main() {

	// get temperature from TMP102
	int handle;
	int address;
	char buf[10] = { 0 };
	float data;
	char channel;

	address = 0x48;
	handle = openDevice(0);
	ioctl(handle, I2C_SLAVE, address);

	for (int i = 0; i<4; i++) {     // Using I2C Read
		if (read(file,buf,2) != 2) {
			/* ERROR HANDLING: i2c transaction failed */
			printf("Failed to read from the i2c bus.\n");
			buffer = g_strerror(errno);
			printf(buffer);
			printf("\n\n");
		} else {
			data = (float)((buf[0] & 0b00001111)<<8)+buf[1];
			data = data/4096*5;
			channel = ((buf[0] & 0b00110000)>>4);
			printf("Channel %02d Data:  %04f\n",channel,data);
		}
	}


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

	/*PART 2 - COMPLETE SECOND AND THIRD OBJECTIVES*/

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
