#include "gpio.h"

int openGPIO(int gpio, int direction) {
	int handle;               //file variable
	char buf[256];
	int inout;

	//simple command to enable pin A0

	handle = open("/sys/class/gpio/export", O_WRONLY);

	sprintf(buf, "%d", gpio);

	write(handle, buf, strlen(buf));

	close(handle);

	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);

	handle = open(buf, O_WRONLY);

	// Set out direction
	switch (direction) {
	case GPIO_DIRECTION_OUT:
		write(handle, "out", 3);
		inout = O_WRONLY;
		break;
	case GPIO_DIRECTION_IN:
	default:
		write(handle, "in", 2);
		inout = O_RDONLY;
		break;

	}
	// write(handle, "out", 3);
	// Set in direction


	close(handle);

	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);

	switch (direction) {
	case GPIO_DIRECTION_OUT:
		handle = open(buf, inout);
		break;
	case GPIO_DIRECTION_IN:
	default:
		handle = open(buf, inout);
	}
	// handle = open(buf, O_WRONLY);

	return handle;

}

//Read value on the GPIO pins
int readGPIO(int handle) {
	char ret;
	read(handle, &ret, 1);
	return (ret - '0');
}
//write value on the GPIO pins
void writeGPIO(int handle, int status_write) {

	// Set GPIO high status
	if (status_write) {
		write(handle, "1", 1);
	} else {
		write(handle, "0", 1);
	}

	return 0;
}

void closeGPIO(int gpio, int handle) {
	char BUFFER[255];

	close(handle);

	handle = open("/sys/class/gpio/unexport", O_WRONLY);
	sprintf(BUFFER, "%d", gpio);
	write(handle, BUFFER, strlen(BUFFER));
	close(handle);
	return;
}
