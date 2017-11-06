//***i2c libraries**
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

//*** Opencv libraries ***
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>

//*** Normal c/c++ code libraries ***
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
//#include <cstdio>
//#include <cstdlib>
//#include <ctime>
#include <unistd.h>

// mine
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void takePicture() {

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

	/*PART 1 - create filename*/
	sprintf(filename, "/home/root/Documents/to PC/fuck you.jpg");
	// SD drive is at /media/card/ etc.
	/*END PART 1*/

	/*PART 2 - capture image*/
	capture = cvCaptureFromCAM(CV_CAP_ANY);	// what is CV_CAP_ANY ???
	/*END PART 2*/

	/*PART 3 - create image structure*/
	image = cvQueryFrame(capture);
	/*END PART 3*/

	/*PART 4 - save image to file*/
	cvSaveImage(filename, image, 0);		// I added third argument because C does not have default arguments support
	/*END PART 4*/

	/*PART 5 - de-initialise camera*/
	cvReleaseCapture(&capture);		// this might be wrong
	/*END PART 5*/

	return;
}

int main() {
	takePicture();
	return 0;
}