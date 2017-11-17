#include "PIC.h"
#include "gpio.h"

/* Code from "/Lab 2/main.c" used */

/*Strobe (40) pin 8
GP_4 LSB and GP_7 MSB for data bus (A0-A3)
*/

/*
Main Program for galileo
prints options and accepts command (with error checking)
command is sent to PIC

Strobe protocol:

galileo sets bus as output (PIC should be inputs already)
galileo puts data on bus
galileo raises strobe
galileo pauses for 10 ms and gives PIC time to read data on bus

repeat this for every nibble that must be read:
	galileo lowers strobe signal
	galileo makes pins inputs
	galileo pauses for 2 ms so that PIC has time to form response
	galileo raises strobe signal
	galileo pauses for 2 ms so that PIC has time to switch pins to outputs
	galileo reads data on bus

galileo lowers strobe signal
*/

int initialiseCommunicay() {
	
	int strobe;										// handle for strobe signal
	unsigned int temp;
	int status;
	
	strobe = openGPIO(Strobe, GPIO_DIRECTION_OUT);	// always out!
	writeGPIO(strobe, LOW);                         // make strobe low
	
	// open strobe. Leave it open and continuously overwrite every call.
	
	// make sure that communication has been established.
	status = sendCommand(strobe, MSG_PING, &temp);
	if (temp != MSG_ACK || status) {
		// error in communication
		// close GPIO port and return error
		closeGPIO(Strobe, strobe);
		return -1;
		// Debug problems: reset PIC with that pin so that phases line up
	}
	
	return strobe;   // success
}

unsigned int sendCommand(int strobe, unsigned char input) {
	
	int flag;
	unsigned int response;
	
	int data [4];			// handles for data bus pins (A3-A0)
	
	writeGPIO(strobe, LOW); // should already be low
	
	/*
	START STEP 1
		1) open all pins as outputs
		2) put data on bus
		3) flip strobe on
		4) Give pic 10ms to read data
	*/
	
	// 1
	data[0] = openGPIO(GP_4, GPIO_DIRECTION_OUT);
	data[1] = openGPIO(GP_5, GPIO_DIRECTION_OUT);
	data[2] = openGPIO(GP_6, GPIO_DIRECTION_OUT);
	data[3] = openGPIO(GP_7, GPIO_DIRECTION_OUT);
	
	writeBus(input & 0xF, data);		// 2
	writeGPIO(strobe, HIGH);		// 3
	usleep(10000);					// 4
	/*END STEP 1*/

	/*STEP 2 -- read data from PIC*/
	flag = 0;
	response = 0;
	while ((flag < 4 && input == MSG_GET) || flag < 1) {
		// if msg_get, read 4 times
		// else, just read response
			/*
		READ FROM PIC
		1) bring strobe low
		2) remove data from bus
		3) make pins inputs after closing them
		4) give PIC some auxiliary some extra time to generate response
		5) raise strobe high
		6) give PIC time to convert pins from inputs to outputs
		7) read bus
		*/
		
		writeGPIO(strobe, LOW);				// 1
		writeBus(0, data);						// 2

		// 3
		closeGPIO(GP_4, data[0]);
		closeGPIO(GP_5, data[1]);
		closeGPIO(GP_6, data[2]);
		closeGPIO(GP_7, data[3]);
		data[0] = openGPIO(GP_4, GPIO_DIRECTION_IN);
		data[1] = openGPIO(GP_5, GPIO_DIRECTION_IN);
		data[2] = openGPIO(GP_6, GPIO_DIRECTION_IN);
		data[3] = openGPIO(GP_7, GPIO_DIRECTION_IN);
		
		usleep(2000);						// 4
		writeGPIO(strobe, HIGH);			// 5
		usleep(2000);						// 6

		// 7
		if (input == MSG_GET) {
			response += readBus(data) << (4 * (3 - flag));	// 7 + extra
		} else {
			response = readBus(data);
		}

		++flag;     // increment counter

	}   // end loop
	/*END STEP 2*/
	
	/*START STEP 3 -- just switch strobe to low to indicate that communication is over, and close pins*/
	writeGPIO(strobe, LOW);
	closeGPIO(GP_4, data[0]);
	closeGPIO(GP_5, data[1]);
	closeGPIO(GP_6, data[2]);
	closeGPIO(GP_7, data[3]);
	/*END STEP 3*/
	
	return response;
}

void endCommunicay(int strobe) {
	closeGPIO(Strobe, strobe);
}

