#include <stdio.h>
#include "i2c.h"

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
}

double readTemp(int handle) {
	// return temperature in celsius

	char buffer[2];				// bytes that shall be read will go here
	int temp;

	read(handle, buffer, 2);							// read 2 bytes from temperature register
	temp = (buffer[0] << 4) | (buffer[1] >> 4);			// shift bytes by appropriate amounts to get 12-bit value
	return (double)temp * 0.0625;						// multiply by 0.0625 (2^-4) [the resolution] to get temperature in celsius
}
