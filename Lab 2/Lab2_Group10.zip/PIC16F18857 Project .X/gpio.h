#ifndef GPIO_H
#define	GPIO_H

/*GPIO Constants*/
// 4-bit instructions from computer
#define MSG_RESET 0x0			// reset the sensor to initial state
#define MSG_PING 0x1			// check if the sensor is working properly
#define MSG_GET 0x2				// obtain the most recent ADC result
#define MSG_TURN30 0x3			// turn the servo motor blade 30 degrees
#define MSG_TURN90 0x4			// turn the servo motor blade 90 degrees
#define MSG_TURN120 0x5			// turn the servo motor blade 120 degrees
// 4-bit responses from PIC
#define MSG_ACK 0xE				// acknowledgement to the commands
#define MSG_NOTHING 0xF			// reserved

// globals
int communication_counter;      // communication level for ISR
int instruction;                // instruction read

/*Initialise GPIO communication w/ computer*/
void GPIO_Init();
// set up interrupt for messages

int read();                 // set GPIO pins to inputs and read their value
void write(int);            // set GPIO pins to outputs and write value

#endif	/* GPIO_H */
