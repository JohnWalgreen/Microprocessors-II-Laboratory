#include <linux/i2c-dev.h>		// access i2c adapter from linux program; this may be incorrect library
#define ADAPTER_NUMBER 0		// is determined dynamically [inspect /sys/class/i2c-dev/ or run "i2cdetect -l" to decide this.]

#define DEST_FOLDER "/home/root/Documents/to PC"		// pictures end up here; SD drive is at /media/card/ etc.

/*STUFF I STOLE FROM INTERNET*/
//#include <glib.h>
//#include <glib/gprintf.h>
//#include <errno.h>

//#include <string.h>

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

//*** Normal c/c++ code libraries ***
//#include <fcntl.h>

//#include <stdint.h>

//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <string.h>

//#include <math.h>

//#include <unistd.h>

// mine
//#include <stdio.h>
//#include <stdlib.h>

//#include <time.h>
/*END*/

#include "i2c.h"


int main() {

	int handle;
	char buffer[200];
	FILE *file;
	unsigned int i, j, counter;
	double t;

	handle = InitTempDevice(ADAPTER_NUMBER);

	// measure noise
	printf("Keep your filthy paws away from the sensor.");
	sprint(buffer, "%s/noise.csv", DEST_FOLDER);
	file = fopen(buffer, "w");
	if (file == NULL) {
		fputs("\nError opening noise.csv", stderr);
		return 0;
	}
	// 10 seconds / 10 000 iterations = 1/1000 seconds per iteration or 1 ms per iteration
	fprintf("time (seconds),temperature (Celsius)\n");
	for (i = 0; i < 10000; i++) {
		fprintf(file, "%u,%2.4lf\n", (double)i / 1000, readTemp(handle));
		usleep(1000);
	}
	fclose(file);
	sleep(5);

	// measure short, infrequent taps
	sprintf(buffer, "%s/infrequent taps.csv", DEST_FOLDER);
	file = fopen(buffer, "w");
	if (file == NULL) {
		fputs("\nError while opening infrequent taps.csv", stderr);
	}
	printf("\rOkay fucktard, get ready.  When I say \"GO\", gently tap the sensor every two seconds until I tell you to stop.");
	sleep(10);
	printf("\rGO!!!!!");
	sleep(3);
	fprintf("time (seconds),temperature (Celsius)\n");
	for (i = 0; i < 10000; i++) {
		fprintf(file, "%u,%2.4lf", (double)i / 1000, readTemp(handle));
		usleep(1000);//1 ms
	}
	printf("/rSTOP!");
	fclose(file);
	sleep(5);

	// measure frequent taps
	sprintf(buffer, "%s/frequent taps.csv", DEST_FOLDER);
	file = fopen(buffer, "w");
	if (file == NULL) {
		fputs("Error while opening frequent taps.csv", stderr);
		return 0;
	}
	printf("\rOkay asshole, when I say \"GO\", gently tap the sensor once per second until I tell you to stop.");
	sleep(10);
	printf("\rGO!!!!!!!!!!!!!");
	sleep(3);
	fprintf("time (seconds),temperature (Celsius)\n");
	for (i = 0; i < 10000; i++) {
		fprintf(file, "%u,%2.4lf", (double)i / 1000, readTemp(handle));
		usleep(1000);//1 ms
	}
	printf("/rSTOP!");
	fclose(file);
	sleep(5);

	// measure long presses
	sprintf(buffer, "%s/long presses.csv", DEST_FOLDER);
	file = fopen(buffer, "w");
	if (file == NULL) {
		fputs("Error opening long presses.csv", DEST_FOLDER);
		return 0;
	}
	fprintf("time (seconds),temperature (Celsius),touching?\n");
	printf("\rNow pay attention.\nWhen I say \"GO\", put your finger on the sensor.\nWhen I say \"STOP\", take it off the sensor.\nThis will repeat multiple times, so when I say \"DONE\", the test is over.\nIt is extremely important that you follow directions as promptly as possible.\n\n");
	sleep(20);
	printf("Get ready");
	sleep(5);
	printf("\rGET SET...");
	sleep(5);

	counter = 0;
	for (j = 1; j <= 10; j++) {
		t = readTemp(handle);
		printf("\rGO!");
		for (i = 0; i < (j * 1000); i++) {
			fprintf(file, "%u,%2.4lf,1", (double)counter / 1000, readTemp(handle));
			++counter;
			usleep(1000);//1 ms
		}
		t = ((1 - 0.9) * (readTemp(handle) - t)) + t;		// 90% cooling
		printf("\rSTOP! Cooling right now. Wait.");
		
		while (readTemp(handle) > t) {
			fprintf(file, "%u,%2.4lf,0", (double)counter / 1000, readTemp(handle));
			++counter;
			usleep(1000);//1 ms
		}
	}

	printf("\rDONE.\n\nThank you for doing this test.\nMove all CSV files to a PC and process data.\n\n");
	fclose(file);

	return 0;
} // end main
