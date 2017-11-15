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
Time: 1015 - Later
Description:
				1) We ran "i2cdetect -l" and saw i2c-0
				2) We rand "i2cdetect -r 0" and got a table with something showing up at address 0x48 (the TMP102).  So it is connected via I2C.
				3) We read temperature

Name: Hans
Date: 6-Nov-2017
Time: 1230 - ...
Description:			4) Discovered that OpenCV is already installed on Galileo
						2) Made tutorial to connect Galileo via ethernet
						1) made batch files on both devices to easily transfer files
						3) Got C++ compiling statement and C++ skeleton code
						5) Manipulated code to make it better and made it okay for gcc (command in makefile)
						6) Can take pictures now with make take_picture_c
						7) added and fixed take picture function and put it in here
						8) TO DO: put files in SD card instead!
						9) TO DO: figure out take picture function and clean up {???} comments
						10) TO DO: this exits after first ten pictures.  In reality, change this
						11) TO DO: Better threshold calculations
						12) dynamic threshold fixed!

Name: Hans
Date: 7-Nov-2017
Time: 1330 - ...
Description:			1) Got new camera
						2) Acquired data
						3) dynamic threshold work via peak detecting
*/

/*
Lab objectives from provided materials:
	1) program I2C devices from Linux. Use Linux I2C libraries and APIs
	2) programming on Linux to handle webcam and capture images. Store images on SD card.
	3) Use temperature sensor to trigger capture of images from webcam.
*/

#include <linux/i2c-dev.h>		// access i2c adapter from linux program; this may be incorrect library

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "i2c.h"
#include "pic.h"

int main() {

	unsigned int pic_counter;		// # of pictures taken
	int temp_sensor_handle;
	double temp, temp_threshold;


	/*declare and initialise variables here*/

	/* PART 1 - COMPLETE FIRST OBJECTIVE */
	// Note: I2C on A4 (SDA) and A5 (SCL) of galileo
	temp_sensor_handle = InitTempDevice(ADAPTER_NUMBER);	// get I2C handle to temp sesnor
	pic_counter = 0;

	/* protocol to determine temperature threshold dynamically */
	puts("Get ready to put hand on the sensor...");
	sleep(5);
	puts("Put hand on temperature sensor. Do not remove until instructed to do so.");
	sleep(5);
	temp_threshold = readTemp(temp_sensor_handle);
	puts("Now take your hand off the sensor.");

	printf("Threshold: %2.2lf degrees Celsius\nProgram will begin in 5 seconds...\n\n", temp_threshold);
	sleep(5);

	/* PART 2 - COMPLETE SECOND AND THIRD OBJECTIVES */
	// infinite loop - exit from inside
	while (1) {

		temp = readTemp(temp_sensor_handle);					// read temperature via I2C to temp sensor

		if (temp > temp_threshold) {
			// temperature is above threshold, so take picture and update counter

			++pic_counter;
			printf("\rYour picture is being taken. Temperature (C) = %2.2lf\n_", temp);
			takePicture(pic_counter);		// stores as [pic_counter value].jpg
			
			if (pic_counter >= PICTURE_LIMIT) {
				// if enough pictures have been taken, exit
				return 0;
			}

		} else {
			// if temperature is not above threshold, overwrite line with current 5temperature
			printf("\r%2.2lf", temp);
		}


	}

	return 0;		// the code shall never reach this point

} // end main
