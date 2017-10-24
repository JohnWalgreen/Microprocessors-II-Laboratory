/*
Galileo communication
Umass Lowell
Derek Teixeira
Created 10/14/17 at 11:10 am


Second Edit was done on 10/21/2017 at 14:56
Able to get mostly everything started but pwm from Galileo

*/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>


//Linux GPIO number///  Adruino shield Pin Name

#define Strobe    (40)    //8
#define GP_4      (48)    //A0
#define GP_5      (50)    //A1
#define GP_6      (52)    //A2
#define GP_7      (54)    //A3
#define GPIO_DIRECTION_IN     (1)
#define GPIO_DIRECTION_OUT    (0)
#define ERROR                 (-1)
#define HIGH 1
#define LOW 0
#define MSG_RESET   0X0      //RESET THE SENSOR TO INTIAL STATE
#define MSG_PING    0X1      //CHECK IF THE SENSOR IS WORKING PROPERLY
#define MSG_GET     0X2      //OBTAIN THE MOST RECENT ADC RESULT
#define MSG_TURN30  0X3      //TURN SERVO MOTOR 30 DEGREES
#define MSG_TURN90  0X4      //TURN SERVO MOTOR 90 DEGRESS
#define MSG_TURN120 0X5      //TURN SERVO MOTOR 120 DEGRESS
#define MSG_ACK     0XE      //ACKNOWLEDMENT TO THE COMMANDS
#define MSG_NOTHING 0XF      //RESERVED



//main
int main(void)
{
	/*START OPEN GPIO*/
	int handle;               //file variable
	char buf[256];
	int gpio = GP_4;
	printf("HELLO WORLD!");

	//simple command to enable pin A0

	handle = open("/sys/class/gpio/export", O_WRONLY);

	sprintf(buf, "%d", gpio);

	write(handle, buf, strlen(buf));

	close(handle);

	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);

	handle = open(buf, O_WRONLY);

	write(handle, "in", 2);
	// Set in direction


	close(handle);

	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);

	handle = open(buf, O_RDONLY);

	/*END OF OPEN GPIO*/


	/*START READ GPIO*/
	char ret;
	read(handle, &ret, 1);
	printf("Value that was read: %d", (int)(ret - '0'));
	/*END READ GPIO*/


	/*START CLOSE GPIO*/
	/*
	char BUFFER[255];

	close(handle);

	handle = open("/sys/class/gpio/unexport", O_WRONLY);
	sprintf(BUFFER, "%d", gpio);
	write(handle, BUFFER, strlen(BUFFER));
	close(handle);
	/*
	/*END CLOSE GPIO*/

	return 0;
}