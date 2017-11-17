#ifndef WEBCAM_H_
#define WEBCAM_H_

/* Taken from "Lab 3/pic.h" */

// Open CV Header Files
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define DEST_FOLDER "/media/card/to PC"		// pictures end up here; SD drive is at /media/card/ etc.
#define PICTURE_LIMIT 5						// maximum number of pictures that will be taken

void takePicture(unsigned int id);

#endif
