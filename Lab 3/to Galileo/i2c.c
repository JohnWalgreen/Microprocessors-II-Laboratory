#include <stdio.h>
#include "i2c.h"

/*STUFF I STOLE FROM INTERNET*/
//#include <glib.h>
//#include <glib/gprintf.h>
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

int InitTempDevice(int adapter_number) {
	// opens i2c device and returns file handle
	// returns <0 if error

	int handle;
	char filename[50];

	sprintf(filename, "/dev/i2c-%d", adapter_number);
	handle = open(filename, O_RDWR);					// gets handle to temperature sensor
	ioctl(handle, I2C_SLAVE, ADDRESS);
	write(handle, 0, 1);								// set pointer register byte to zero so that it acts as read-only temperature sensor
	return handle;

	// HELPER CODE
	/*int file;
	int adapter_nr = 2; // probably dynamically determined
	char filename[20];

	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
	file = open(filename, O_RDWR);
	if (file < 0) {
		// ERROR HANDLING; you can check errno to see what went wrong
		
		exit(1);
	}*/
}

double readTemp(int handle) {
	// return temperature in celsius
	char buffer[2];				// read bytes
	int temp;

	read(handle, buffer, 2);				// read 2 bytes from temperature register
	temp = (buffer[0] << 4) + (buffer[1] >> 4);	// shift bytes by appropriate amounts to get 12-bit value
	return (double)temp * 0.0625;						// multiply by 0.0625 (2^-4) [the resolution] to get temperature in celsius
}

double determineTempThreshold(int handle) {
	unsigned int i;
	double min, max;
	double temp;

	min = (double)((int)1 << 12);			// maximum possible temperature
	max = 0.0;								// minimum possible temperature

	for (i = 0; i < NUM_SAMPLES; i++) {
		// read samples and update variables
		temp = readTemp(handle);
		if (temp > max) {
			max = temp;
		}
		if (temp < min) {
			min = temp;
		}
		sleep(0.1);
	}

	// return value = average + multiplier * range
	// designed for increased temperatures to trigger an event
	return ((max + min) / 2) +
		((max - min) * MULTIPLIER);
}