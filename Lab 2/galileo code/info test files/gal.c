
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define Strobe (26) // IO8
#define GP_4 (28) // IO4
#define GP_5 (17) // IO5
#define GP_6 (24) // IO6
#define GP_7 (27) // IO7
#define GPIO_DIRECTION_IN (1)
#define GPIO_DIRECTION_OUT (0)
#define ERROR (-1)
#define HIGH 1
#define LOW 0
#define MSG_RESET 0x0 /* reset the sensor to initial state */
#define MSG_PING 0x1 /* check if the sensor is working */
#define MSG_GET 0x2 /* obtain the most recent ADC result */
#define MSG_ACK 0xE /* acknowledgement to the commands */
#define MSG_NOTHING 0xF /* reserved */
//open GPIO and set the direction
int openGPIO(int gpio, int direction )
{
	char buffer[256];
    int fileHandle;
    int fileMode;

  	//Export GPIO
    fileHandle = open("/sys/class/gpio/export", O_WRONLY);
    if(ERROR == fileHandle)
     	{
         puts("Error: Unable to opening /sys/class/gpio/export");
         return(-1);
     	}
    sprintf(buffer, "%d", gpio);
    write(fileHandle, buffer, strlen(buffer));
    close(fileHandle);

    //Direction GPIO
    sprintf(buffer, "/sys/class/gpio/gpio%d/direction", gpio);
    fileHandle = open(buffer, O_WRONLY);
    if(ERROR == fileHandle)
        {
            puts("Unable to open file:");
            puts(buffer);
            return(-1);
        }

    if (direction == GPIO_DIRECTION_OUT)
        {
            // Set out direction
            write(fileHandle, "out", 3);
            fileMode = O_WRONLY;
        }
    else
        {
            // Set in direction
            write(fileHandle, "in", 2);
            fileMode = O_RDONLY;
        }
    close(fileHandle);


   //Open GPIO for Read / Write
    sprintf(buffer, "/sys/class/gpio/gpio%d/value", gpio);
    fileHandle = open(buffer, fileMode);
    if(ERROR == fileHandle)
     	{
            puts("Unable to open file:");
            puts(buffer);
            return(-1);
        }

    return(fileHandle);  //This file handle will be used in read/write and close operations.
}
//write value
int writeGPIO(int fileHandle, int val)
{
	if(val == 0)
	{
		// Set GPIO low status
		write(fileHandle, "0", 1);
	}
	else
	{
		// Set GPIO high status
		write(fileHandle, "1", 1);
	}
	return(0);
}

//read value
int readGPIO(int fileHandle)
{
	int value;
	read(fileHandle, &value, 1);
	if('0' == value)
	{
		// Current GPIO status low
		value = 0;
	}
	else
	{
		// Current GPIO status high
		value = 1;
	}
	return value;
}

int closeGPIO(int gpio, int fileHandle)
{
char buffer[256];
close(fileHandle); //This is the file handle of opened GPIO for Read / Write earlier.
fileHandle = open("/sys/class/gpio/unexport", O_WRONLY);
if(ERROR == fileHandle)
{
puts("Unable to open file:");
puts(buffer);
return(-1);
}
sprintf(buffer, "%d", gpio);
write(fileHandle, buffer, strlen(buffer));
close(fileHandle);
return(0);
}

//main
int main(void)
{
struct timespec delay;
   delay.tv_sec = 0;
   delay.tv_nsec = 10;

int adc=0;
int ack=0;
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

writeGPIO(fileHandleGPIO_S,1);
nanosleep(&delay,NULL);
while(1)
{
	//send command
		//1.Strobe high
		writeGPIO(fileHandleGPIO_S,HIGH);
		//strobe low
		writeGPIO(fileHandleGPIO_S,LOW);
		//write COMMAND MSG_GET 
		writeGPIO(fileHandleGPIO_4,LOW);
		writeGPIO(fileHandleGPIO_5,HIGH);
		writeGPIO(fileHandleGPIO_6,LOW);
		writeGPIO(fileHandleGPIO_7,LOW);
		//3.Strobe HIGH
		writeGPIO(fileHandleGPIO_S,HIGH);
		nanosleep(&delay,NULL);

		//4.Strobe low
		writeGPIO(fileHandleGPIO_S,LOW);
		writeGPIO(fileHandleGPIO_S,HIGH);
		closeGPIO(GP_4, fileHandleGPIO_4);
		closeGPIO(GP_5, fileHandleGPIO_5);
		closeGPIO(GP_6, fileHandleGPIO_6);
		closeGPIO(GP_7, fileHandleGPIO_7);
		fileHandleGPIO_4 = openGPIO(GP_4, GPIO_DIRECTION_IN);
		fileHandleGPIO_5 = openGPIO(GP_5, GPIO_DIRECTION_IN);
		fileHandleGPIO_6 = openGPIO(GP_6, GPIO_DIRECTION_IN);
		fileHandleGPIO_7 = openGPIO(GP_7, GPIO_DIRECTION_IN);

	// get data
		// adc 0-3
		writeGPIO(fileHandleGPIO_S,HIGH);
		writeGPIO(fileHandleGPIO_S,LOW);
		nanosleep(&delay,NULL);
		writeGPIO(fileHandleGPIO_S,HIGH);
		adc += pow (2,0)*readGPIO(fileHandleGPIO_4);
		adc += pow (2,1)*readGPIO(fileHandleGPIO_5);
		adc += pow (2,2)*readGPIO(fileHandleGPIO_6);
		adc += pow (2,3)*readGPIO(fileHandleGPIO_7);

		// adc 4-7
		writeGPIO(fileHandleGPIO_S,HIGH);
		writeGPIO(fileHandleGPIO_S,LOW);
		nanosleep(&delay,NULL);
		writeGPIO(fileHandleGPIO_S,HIGH);
		adc += pow (2,4)*readGPIO(fileHandleGPIO_4);
		adc += pow (2,5)*readGPIO(fileHandleGPIO_5);
		adc += pow (2,6)*readGPIO(fileHandleGPIO_6);
		adc += pow (2,7)*readGPIO(fileHandleGPIO_7);

		// adc 4-7
		writeGPIO(fileHandleGPIO_S,HIGH);
		writeGPIO(fileHandleGPIO_S,LOW);
		nanosleep(&delay,NULL);
		writeGPIO(fileHandleGPIO_S,HIGH);
		adc += pow (2,8)*readGPIO(fileHandleGPIO_4);
		adc += pow (2,9)*readGPIO(fileHandleGPIO_5);

		// msg ack
		writeGPIO(fileHandleGPIO_S,HIGH);
		writeGPIO(fileHandleGPIO_S,LOW);
		nanosleep(&delay,NULL);
		writeGPIO(fileHandleGPIO_S,HIGH);
		ack += pow (2,0)*readGPIO(fileHandleGPIO_4);
		ack += pow (2,1)*readGPIO(fileHandleGPIO_5);
		ack += pow (2,2)*readGPIO(fileHandleGPIO_6);
		ack += pow (2,3)*readGPIO(fileHandleGPIO_7);

		writeGPIO(fileHandleGPIO_S,LOW);
		writeGPIO(fileHandleGPIO_S,HIGH);
		if (ack==15)
		{
			printf("adc receieved\n");
			printf ("adc is %d",adc);
		}
		else
		{
			printf("adc error");
		}
		
}
}

