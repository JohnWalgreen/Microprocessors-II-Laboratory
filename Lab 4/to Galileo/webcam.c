/* Taken from "/Lab 3/pic.c" */
// updated filename

#include "webcam.h"

// Open CV Header Files
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <stdio.h>

void takePicture(unsigned int id) {

	char filename[200];
	CvCapture *capture;
	IplImage *image;

	/*
		1) Establish file name
		2) capture frame
		3) retrieve data from frame
		4) save image to file
		5) release capture
		6) release image
	*/

	sprintf(filename, "%s/image%u.jpg", DEST_FOLDER, id);	// filename is [DEST_FOLDER]/[id].jpg
	capture = cvCaptureFromCAM(CV_CAP_ANY);			// capture frame from the camera; stop webcam
								// argument can be zero since there is only one device connected
	image = cvQueryFrame(capture);				// grabs and retrieves data from captured frame
	cvSaveImage(filename, image, 0);			// save image to file as JPG
	cvReleaseCapture(&capture);				// release capture
	cvReleaseImage(&image);					// release image

	return;
}
