/*
* File:   PIC and Galileo communication
*
* simple Galileo program example
* for UMass Lowell 16.480/552
*
* Author: Ioannis
*
* Created on 2017/9/21
*/


#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


//Linux GPIO number // Arduino Shield Pin Name
#define HIGH					(1)
#define LOW						(0)
#define Strobe					(40) // 8
#define GP_4					(48) // A0
#define GP_5					(50) // A1
#define GP_6					(52) // A2
#define GP_7					(54) // A3
#define GPIO_DIRECTION_IN		(1)  
#define GPIO_DIRECTION_OUT		(0)
#define ERROR					(-1)

// COMMANDS TO PIC
#define MSG_RESET				(0x0) // Reset sensor to initial state
#define MSG_PING				(0x1) // Check if sensor is working properly
#define MSG_GET					(0x2) // Get ADC reading
#define MSG_TURN30				(0x3) // Turn 30*
#define MSG_TURN90				(0x4) // Turn 90*
#define MSG_TURN120				(0x5) // Turn 120*

// SENSOR DEVICE RESPONSES
#define MSG_ACK					(0xE) // Acknowledgement to the commands
#define MSG_NOTHING				(0xF) // Reserved

// Open GPIO and set the direction.
// Thanks to the file from http://www.malinov.com/Home/sergey-s-blog/intelgalileo-programminggpiofromlinux for the necessary sprintf to create the files for each individual pin. 
// Remember to open and close files between use.
int openGPIO(int gpio, int direction)
{
	char buffer[256];
	int file;
	int readwrite;

	// Set GPIO
	file = fopen("sys/class/gpio/export", "w");							// Open the file to write from
	sprintf(buffer, "%d", gpio);										// Print the file to the buffer
	write(file, buffer, strlen(buffer));								// Write the file from filehandle into the buffer so that we can close the file. 
	fclose(file);														// Close the file.

	// Set the direction

	sprintf(buffer, "sys/class/gpio/gpio%d/direction", gpio);			// Create the file "gpio(number)/direction, as per the linux type in the system file. 
	file = fopen(buffer, "w");											// Open the file from buffer (previous file handle) into the system. 
	if (direction == GPIO_DIRECTION_IN)									// IF: The direction is set to be in, make the readwrite variable be set to read only. This will be used later to set 
																		// the direction of the pin to be forced in.
	{
		write(file, "in", 2);			// Write "in" to the file.
		readwrite = "r";				// Set readwrite to "r"
	}
	else if (direction == GPIO_DIRECTION_OUT)							// IF: set to be out, set the readwrite variable to be write only! this will make the direction out.
	{
		write(file, "out", 3);			// Write "out" to the file.
		readwrite = "w";				// Set readwrite to "w"
	}
	fclose(file);														// Close the current filehandle.
	sprintf(buffer, "sys/class/gpio/gpio%d/value", gpio);				// Create the "value" file for the GPIO. 			
	file = open(buffer, readwrite);										// Open and return the value of the current gpio.

	// Voltage is preset to 3.3V input/output via the switch on the program.	

	return(file);														// Return the value.
}

// Write value to a specific pin
void writeGPIO(int gpio, int value)										// Take in the variable for the file
{
	if (value == 0)														// Set pin to low if the value is low.
	{
		digitalWrite(gpio, LOW);										
	}
	else if(value == 1)													// Set pin to high if the value is high.
	{
		digitalWrite(gpio, HIGH);
	}
}

// Read value from a specific pin
int readGPIO(int gpio)
{
	
	return(digitalRead(gpio));											// Return the value read from the filehandle pin itself.

}


//main
int main(void)
{
	int direction;
	int gpio;
	int fileHandleGPIO_4;
	int fileHandleGPIO_5;
	int fileHandleGPIO_6;
	int fileHandleGPIO_7;
	int fileHandleGPIO_S;
	fileHandleGPIO_4 = openGPIO(GP_4, GPIO_DIRECTION_OUT);
	fileHandleGPIO_5 = openGPIO(GP_5, GPIO_DIRECTION_OUT);
	fileHandleGPIO_6 = openGPIO(GP_6, GPIO_DIRECTION_OUT);
	fileHandleGPIO_7 = openGPIO(GP_7, GPIO_DIRECTION_OUT);
	fileHandleGPIO_S = openGPIO(Strobe, GPIO_DIRECTION_OUT);

	while (1)
	{
		// 1.Strobe high
		writeGPIO(fileHandleGPIO_S, HIGH);
		// 2. GET DATA;
		












			
		// 3.Strobe low
		writeGPIO(fileHandleGPIO_S, LOW);
		// 4.Strobe high
		writeGPIO(fileHandleGPIO_S, HIGH);
	}
}
