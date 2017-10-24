/*
Hans
built from gpion
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


//open GPIO and set direction

int openGPIO(int gpio, int direction)
{
	int handle;               //file variable
	char buf[256];
	int inout;

						  //simple command to enable pin A0

	handle = open("/sys/class/gpio/export", O_WRONLY);

	sprintf(buf, "%d", gpio);

	write(handle, buf, strlen(buf));

	close(handle);

	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);

	handle = open(buf, O_WRONLY);

	// Set out direction
	switch (direction) {
		case GPIO_DIRECTION_OUT:
			write(handle, "out", 3);
			inout = O_WRONLY;
			break;
		case GPIO_DIRECTION_IN:
		default:
			write(handle, "in", 2);
			inout = O_RDONLY;
			break;

	}
	// write(handle, "out", 3);
	// Set in direction


	close(handle);

	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);

	switch (direction) {
		case GPIO_DIRECTION_OUT:
			handle = open(buf, inout);
			break;
		case GPIO_DIRECTION_IN:
		default:
			handle = open(buf, inout);
	}
	// handle = open(buf, O_WRONLY);

	return handle;

}

    //Will close conection of GPIO when the pin is not in use




    //Read value on the GPIO pins
int readGPIO(int handle)
{
	char ret;
	read(handle, &ret, 1);
	return (ret - '0');
	/*
    int status_read;
    read(fd, &status_read, 1);
    if('0' == status_read)
      {
        status_read = 0;
      }
      else
      {
        status_read = 1;
      }
      return status_read;*/
}
    //write value on the GPIO pins
int writeGPIO(int handle, int status_write)
{

	// Set GPIO high status
	if (status_write) {
		write(handle, "1", 1);
	} else {
		write(handle, "0", 1);
	}

	return 0;
}

void closeGPIO(int gpio, int handle) {
	char BUFFER[255];

	close(handle);

	handle = open("/sys/class/gpio/unexport", O_WRONLY);
	sprintf(BUFFER, "%d", gpio);
	write(handle, BUFFER, strlen(BUFFER));
	close(handle);
	return;
}


//main
int main(void)
{
      int i;
	  int j, k;
      int fileHandleGPIO_4;
      //int fileHandleGPIO_5;
      //int fileHandleGPIO_6;
      //int fileHandleGPIO_7;
      int fileHandleGPIO_S;

      fileHandleGPIO_4 = openGPIO(GP_5, GPIO_DIRECTION_IN);
      //fileHandleGPIO_5 = openGPIO(GP_5, GPIO_DIRECTION_OUT);
      //fileHandleGPIO_6 = openGPIO(GP_6, GPIO_DIRECTION_OUT);
      //fileHandleGPIO_7 = openGPIO(GP_7, GPIO_DIRECTION_OUT);
	  
      fileHandleGPIO_S = openGPIO(Strobe, GPIO_DIRECTION_OUT);

	  for (i = 0; i < 15; i++) {
		  writeGPIO(fileHandleGPIO_S, HIGH);
		  sleep(2);
		  writeGPIO(fileHandleGPIO_S, LOW);
		  sleep(2);
	  }

	  for (i = 0; i < 60; i++) {
		  j = readGPIO(fileHandleGPIO_4);
		  printf("Value #%d: %d\n", i, j);
		  if (j) {
			  writeGPIO(fileHandleGPIO_S, HIGH);
		  } else {
			  writeGPIO(fileHandleGPIO_S, LOW);
		  }
		  
		  sleep(1);
		  /*if (readGPIO(fileHandleGPIO_4)) {
			  writeGPIO(fileHandleGPIO_S, HIGH);
		  } else {
			  writeGPIO(fileHandleGPIO_S, LOW);
		  }*/
	  }
      
	  
      //writeGPIO(fileHandleGPIO_6, HIGH);
        

	  //close(fileHandleGPIO_4);
	  //close(fileHandleGPIO_5);
	  //close(fileHandleGPIO_6);
	  //close(fileHandleGPIO_7);
	  //close(fileHandleGPIO_S);

	  closeGPIO(Strobe, fileHandleGPIO_S);
	  closeGPIO(GP_5, fileHandleGPIO_4);


	  return 0;
}