#ifndef I2C_H_
#define I2C_H_

#define ADDRESS 0x48
#define ADAPTER_NUMBER 0		// is determined dynamically [inspect /sys/class/i2c-dev/ or run "i2cdetect -l" to decide this.]
#define NUM_SAMPLES 10

/*
I ran "i2cdetect -l", saw i2c-0 and now we know we are
using ADAPTER_NUMBER 0 when calling InitTempDevice from
main.

To get ADDRESS, use "i2cdetect -r 0" (0 is ADAPTER_NUMBER)
to see where address where temperature sensor is connected.
*/

int InitTempDevice(int adapter_number);		// returns file handle to device at adapter number
double readTemp(int handle);				// reads temperature using I2C handle
double sampleTemp(int handle);				// sample temperature to get rid of irregularities

#endif

