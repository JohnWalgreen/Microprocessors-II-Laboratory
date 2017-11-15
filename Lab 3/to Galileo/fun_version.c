#include <linux/i2c-dev.h>		// access i2c adapter from linux program; this may be incorrect library
#define ADAPTER_NUMBER 0		// is determined dynamically [inspect /sys/class/i2c-dev/ or run "i2cdetect -l" to decide this.]

#define DEST_FOLDER "/home/root/Documents/to PC"		// pictures end up here; SD drive is at /media/card/ etc.
#define PICTURE_LIMIT 20

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/*END*/

/*COPIED*/
#include <sys/ioctl.h>

//*** Opencv libraries ***
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "i2c.h"

#define SWEARS {"asshole", "bitch", "bitch-ass bitch", "jackass", "fucktard", "moron", "idiot", "fatass", "shitstain", "shitsky", "motherfucker", "piece of shit", "son of a bitch"}
#define NUM_SWEARS 13
#include <time.h>

void takePicture(unsigned int id);

int main() {

	unsigned int pic_counter;		// # of pictures taken
	int temp_sensor_handle;
	double temp, temp_threshold;
  
  char *swears[NUM_SWEARS] = SWEARS;
  srand(time(NULL));  // prepare for random #s
  
	/*declare and initialise variables here*/

	/*PART 1 - COMPLETE FIRST OBJECTIVE*/

	// set up I2C protocol on devices (program the devices?)
	/*Devices:
		PIC? probably not
		TMP102 (temperature sensor)
		USB webcam
	*/
	// I2C on A4 (SDA) and A5 (SCL) of galileo

	temp_sensor_handle = InitTempDevice(ADAPTER_NUMBER);
	pic_counter = 0;

	/*END PART 1*/

	// FIND PROTOCOL TO MAKE TEMPERATURE DYNAMIC
	{
		double num1, num2;

		//puts("Allow the temperature sensor to cool. Place your hand on it once instructed to do so.");
		//sleep(5);
		//num1 = determineTempThreshold(temp_sensor_handle);

		printf("Hey %s, put hand on temperature sensor. Do not remove until instructed to do so\n", swears[rand() % NUM_SWEARS]);
		sleep(5);
		num2 = determineTempThreshold(temp_sensor_handle);

		printf("Okay %s, now take your hand off the sensor\n", swears[rand() % NUM_SWEARS]);
		sleep(5);

		// I changed this to just the high
		temp_threshold = num2;// ((num1 + num2) / 2) + (num2 - num1);		// average + range = threshold
		printf("Threshold: %2.2lf degrees Celsius\nProgram will begin in 5 seconds...\n\n", temp_threshold);
		sleep(5);
	}

	/*PART 2 - COMPLETE SECOND AND THIRD OBJECTIVES*/

	// infinite loop - maybe add backdoor method of exiting
	while (1) {

		// communicate via I2C with temperature sensor
		// get temperature
		// end I2C communicay
		temp = readTemp(temp_sensor_handle);
		if (temp > temp_threshold) {
			// take picture and update counter

			++pic_counter;
			takePicture(pic_counter);

			printf("\rYour picture has been taken, %s. Temperature (C) = %2.2lf\n_", swears[rand() % NUM_SWEARS], temp);
			
			if (pic_counter >= PICTURE_LIMIT) {
				return 0;
			}

		} else {
			printf("\r%2.2lf", temp);
		}


	}

	/*END PART 2*/

	return 0;
} // end main

void takePicture(unsigned int id) {

	// I added -lm to command line argument for C

	/*
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -Wall "take picture.c" i2c.c -o ./gal.out -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching
	gcc -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -Wall "take picture.c" i2c.c -o ./gal.out -lm -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching
	*/

	char filename[200];
	CvCapture *capture;
	IplImage *image;

	/*
	To take a picture:
	1) Create filename
	2) capture image
	3) create image structure
	4) save image to filename
	5) de-initialise camera
	*/

	sprintf(filename, "%s/%u.jpg", DEST_FOLDER, id);		// step 1
	// filename is [DEST_FOLDER]/[id].jpg

	capture = cvCaptureFromCAM(CV_CAP_ANY);	// step 2; what is CV_CAP_ANY ???
	image = cvQueryFrame(capture);			// step 3
	cvSaveImage(filename, image, 0);		// step 4
	cvReleaseCapture(&capture);				// step 5; why do we use pointer-to-pointer???

	return;
}
