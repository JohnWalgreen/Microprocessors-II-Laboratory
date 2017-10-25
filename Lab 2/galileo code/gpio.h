#ifndef GPIO_H_
#define GPIO_H_

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>


//Linux GPIO number///  Adruino shield Pin Name
/*GPIO Pins*/
#define Strobe    (40)    //8
#define GP_4      (48)    //A0
#define GP_5      (50)    //A1
#define GP_6      (52)    //A2
#define GP_7      (54)    //A3

/*GPIO Directions*/
#define GPIO_DIRECTION_IN     (1)
#define GPIO_DIRECTION_OUT    (0)

/*GPIO valid commands*/
#define MSG_RESET   0X0      //RESET THE SENSOR TO INTIAL STATE
#define MSG_PING    0X1      //CHECK IF THE SENSOR IS WORKING PROPERLY
#define MSG_GET     0X2      //OBTAIN THE MOST RECENT ADC RESULT
#define MSG_TURN30  0X3      //TURN SERVO MOTOR 30 DEGREES
#define MSG_TURN90  0X4      //TURN SERVO MOTOR 90 DEGRESS
#define MSG_TURN120 0X5      //TURN SERVO MOTOR 120 DEGRESS

/*GPIO valid responses*/
#define MSG_ACK     0XE      //ACKNOWLEDMENT TO THE COMMANDS
#define MSG_NOTHING 0XF      //RESERVED

/*Other constants*/
#define ERROR                 (-1)
#define HIGH 1
#define LOW 0

/*Open the GPIO*/
int openGPIO(int gpio, int direction);
// return handle to output/input

/*Write GPIO output*/
void writeGPIO(int handle, int value);
// write value to pin defined by handle

/*Read GPIO input*/
int readGPIO(int handle);
// returns input from pin defined by handle

/*Close GPIO*/
void closeGPIO(int gpio, int handle);

#endif