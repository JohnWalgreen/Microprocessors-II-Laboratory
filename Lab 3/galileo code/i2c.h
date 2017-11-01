#ifndef I2C_H_
#define I2C_H_

// open device file
int openDevice(int adapter_number); // returns file handle

#endif

/*HELPER CODE*

// open device file (taken directly from directions)
/*int file;
int adapter_nr = 2; // probably dynamically determined
char filename[20];

snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
file = open(filename, O_RDWR);
if (file < 0) {
// ERROR HANDLING; you can check errno to see what went wrong

exit(1);
}
*/

// specify with which device address you wish to communicate
/*int addr = 0x40; // The I2C address

if (ioctl(file, I2C_SLAVE, addr) < 0) {
// ERROR HANDLING; you can check errno to see what went wrong
exit(1);
}*/