#ifndef PIC_H_
#define PIC_H_

/* PIC valid commands */
#define MSG_RESET   0x0      //RESET THE SENSOR TO INTIAL STATE
#define MSG_PING    0x1      //CHECK IF THE SENSOR IS WORKING PROPERLY
#define MSG_GET     0x2      //OBTAIN THE MOST RECENT ADC RESULT
#define MSG_TURN30  0x3      //TURN SERVO MOTOR 30 DEGREES
#define MSG_TURN90  0x4      //TURN SERVO MOTOR 90 DEGRESS
#define MSG_TURN120 0x5      //TURN SERVO MOTOR 120 DEGRESS

/* PIC valid responses */
#define MSG_ACK     0xE      //ACKNOWLEDMENT TO THE COMMANDS
#define MSG_NOTHING 0xF      //RESERVED

/**
	* @desc set up communication with PIC
	* @return int - handle to strobe
*/
int initialiseCommunicay();

/**
	* @desc send command to PIC and obtain response. Protect with mutex and semaphore
	* @param int - handle to strobe gpio
	* @param unsigned char - one byte command
	* @return int - response from PIC
*/
unsigned int sendCommand(int strobe, unsigned char command);

/**
	* @desc ends communication with PIC
	* @param int - handle to strobe
*/
void endCommunicay(int strobe);

#endif

