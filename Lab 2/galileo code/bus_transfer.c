#include "bus_transfer.h"
#include "gpio.h

// Warning: is sleep_ms defined? use <<usleep(unsigned int)>> in unistd.h for microseconds

void writeBus(int value, int *bus) {
	writeGPIO(bus[0], value & 0x1);
	writeGPIO(bus[1], (value >> 1) & 0x1);
	writeGPIO(bus[2], (value >> 2) & 0x1);
	writeGPIO(bus[3], (value >> 3) & 0x1);
}

int readBus(int *bus) {
	// readGPIO must return 0 or 1 !!!
	int value = 0;
	value |= (readGPIO(bus[0]));
	value |= (readGPIO(bus[1]) << 1);
	value |= (readGPIO(bus[2]) << 2);
	value |= (readGPIO(bus[3]) << 3);
	return value;
}
