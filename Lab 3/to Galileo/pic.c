void takePicture(unsigned int id) {

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
