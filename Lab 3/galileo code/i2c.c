#include <stdio.h>

int openDevice(int adapter_number) {
	// opens i2c device and returns file handle
	// returns <0 if error

	char filename[50];

	sprintf(filename, "/dev/i2c-%d", adapter_number);
	return open(filename, O_RDWR);

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