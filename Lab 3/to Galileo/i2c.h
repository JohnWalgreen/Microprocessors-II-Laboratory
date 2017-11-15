#ifndef I2C_H_
#define I2C_H_

#define ADDRESS 0x48
#define ADAPTER_NUMBER 0		// is determined dynamically [inspect /sys/class/i2c-dev/ or run "i2cdetect -l" to decide this.]
#define NUM_SAMPLES 10

int InitTempDevice(int adapter_number);		// returns file handle to device at adapter number
double readTemp(int handle);				// reads temperature using I2C handle
double sampleTemp(int handle);				// sample temperature to get rid of irregularities

#endif
