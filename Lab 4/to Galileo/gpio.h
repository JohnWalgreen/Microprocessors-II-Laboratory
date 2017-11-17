#ifndef GPIO_H_
#define GPIO_H_

/* Taken from "/Lab 2/gpio.h" */

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
