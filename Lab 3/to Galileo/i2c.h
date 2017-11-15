#ifndef I2C_H_
#define I2C_H_

#define ADDRESS 0x48

int InitTempDevice(int adapter_number);		// returns file handle to device at adapter number
double readTemp(int handle);				// reads temperature using I2C handle

#endif
