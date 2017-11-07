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

#include <time.h>

/*COPIED*/
#include <sys/ioctl.h>

//*** Opencv libraries ***
//#include <opencv2/objdetect/objdetect.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv/cv.h>
//#include <opencv/highgui.h>

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

#define DELTA_T 5
#define DELAY 0
#define ROLLOVER 100

int main() {

	int handle;
	char buffer[200];
	FILE *file;
	unsigned int i, j, counter;
	double t1, t2;
	clock_t oldstart, start, end;

	handle = InitTempDevice(ADAPTER_NUMBER);

	// measure noise
	printf("Keep your filthy paws away from the sensor.");
	fflush(stdout);
	sprintf(buffer, "%s/noise.csv", DEST_FOLDER);
	file = fopen(buffer, "w");
	if (file == NULL) {
		fputs("\nError opening noise.csv", stderr);
		return 0;
	}
	// 10 seconds / 10 000 iterations = 1/1000 seconds per iteration or 1 ms per iteration
	fprintf(file, "time (seconds),temperature (Celsius)\n");
	start = clock();
	do {
		end = clock();
		if ((end - start) % ROLLOVER == 0) {
			fprintf(file, "%.3lf,%2.4lf\n", (double)(end - start) / (double)CLOCKS_PER_SEC, readTemp(handle));
		}
		usleep(DELAY);
	} while (end - start < DELTA_T * CLOCKS_PER_SEC);
	fclose(file);
	sleep(5);

	// measure short, infrequent taps
	sprintf(buffer, "%s/infrequent taps.csv", DEST_FOLDER);
	file = fopen(buffer, "w");
	if (file == NULL) {
		fputs("\nError while opening infrequent taps.csv", stderr);
	}
	printf("\rOkay fucktard, get ready.  When I say \"GO\", gently tap the sensor every two seconds until I tell you to stop.");
	fflush(stdout);
	sleep(10);
	printf("\rGO!!!!!");
	fflush(stdout);
	sleep(3);
	fprintf(file, "time (seconds),temperature (Celsius)\n");
	start = clock();
	do {
		end = clock();
		if ((end - start) % ROLLOVER == 0) {
			fprintf(file, "%.3lf,%2.4lf\n", (double)(end - start) / (double)CLOCKS_PER_SEC, readTemp(handle));
		}
		usleep(DELAY);
	} while (end - start < DELTA_T * CLOCKS_PER_SEC);
	printf("\rSTOP!");
	fflush(stdout);
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
	fflush(stdout);
	sleep(10);
	printf("\rGO!!!!!!!!!!!!!");
	fflush(stdout);
	sleep(3);
	fprintf(file, "time (seconds),temperature (Celsius)\n");
	start = clock();
	do {
		end = clock();
		if ((end - start) % ROLLOVER == 0) {
			fprintf(file, "%.3lf,%2.4lf\n", (double)(end - start) / (double)CLOCKS_PER_SEC, readTemp(handle));
		}
		usleep(DELAY);
	} while (end - start < DELTA_T * CLOCKS_PER_SEC);
	printf("/rSTOP!");
	fflush(stdout);
	fclose(file);
	sleep(5);

	// measure long presses
	sprintf(buffer, "%s/long presses.csv", DEST_FOLDER);
	file = fopen(buffer, "w");
	if (file == NULL) {
		fputs("Error opening long presses.csv", stderr);
		return 0;
	}
	fprintf(file, "time (seconds),temperature (Celsius),touching?\n");
	printf("\rNow pay attention.\nWhen I say \"GO\", put your finger on the sensor.\nWhen I say \"STOP\", take it off the sensor.\nThis will repeat multiple times, so when I say \"DONE\", the test is over.\nIt is extremely important that you follow directions as promptly as possible.\n\n");
	fflush(stdout);
	sleep(20);
	printf("Get ready");
	sleep(5);
	printf("\rGET SET...");
	sleep(5);

	oldstart = clock();
	for (j = 1; j <= DELTA_T; j++) {
		t1 = readTemp(handle);
		printf("\rGO!");
		fflush(stdout);
		start = clock();
		do {
			end = clock();
			fprintf(file, "%.3lf,%2.4lf\n", (double)(end - oldstart) / (double)CLOCKS_PER_SEC, readTemp(handle));
			usleep(DELAY);
		} while (end - start < j * CLOCKS_PER_SEC);

		t1 = ((1 - 0.9) * (readTemp(handle) - t1)) + t1;		// 90% cooling
		printf("\rSTOP! Cooling right now. Wait.");
		fflush(stdout);
		
		start = clock();
		do {
			end = clock();
			t2 = readTemp(handle);
			fprintf(file, "%.3lf,%2.4lf\n", (double)(end - oldstart) / (double)CLOCKS_PER_SEC, t2);
			usleep(DELAY);
		} while (t2 > t1);
	}

	printf("\rDONE.\n\nThank you for doing this test.\nMove all CSV files to a PC and process data.\n\n");
	fflush(stdout);
	fclose(file);

	return 0;
} // end main
