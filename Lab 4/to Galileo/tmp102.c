/* Taken from "/Lab 3/i2c.c" */

#include <stdio.h>
#include "tmp102.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>          // ioctl function
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int InitTempDevice(int adapter_number) {
	// opens i2c device and returns file handle
	// returns <0 if error

	int handle;         // handle to the temperature sensor; will be returned
	char filename[50];  // to access temperature sensor

	sprintf(filename, "/dev/i2c-%d", adapter_number);
	handle = open(filename, O_RDWR);					// gets handle to temperature sensor
	ioctl(handle, I2C_SLAVE, ADDRESS);                  // io control; set as I2C slave, which is at ADDRESS
	write(handle, 0, 1);								// set pointer register byte to zero so that it acts as read-only temperature sensor
	
	return handle;      // return the handle for further communicay
}

double readTemp(int handle) {
	// return temperature in celsius

	unsigned char buffer[2];							// bytes that shall be read will go here
	unsigned int temp;

	read(handle, buffer, 2);							// read 2 bytes from temperature register
	temp = (buffer[0] << 4) + (buffer[1] >> 4);			// shift bytes by appropriate amounts to get 12-bit value
	return (double)temp * 0.0625;						// multiply by 0.0625 (2^-4) [the resolution] to get temperature in celsius
}

double sampleTemp(int handle) {

	unsigned int i;
	double sum;

	sum = 0;
	for (i = 0; i < NUM_SAMPLES; i++) {
		sum += readTemp(handle);
	}

	return sum / NUM_SAMPLES;
}
