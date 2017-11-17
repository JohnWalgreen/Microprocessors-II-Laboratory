#ifndef THREAD_FUNCTIONS_H_
#define THREAD_FUNCTIONS_H_

/*
This file contains the three thread functions. From main, each function here
will be initialised as a separate thread.
*/

/* This structure is used to pass arguments to thread functions */
struct ThreadArg {
	
};

/* Thread functions. See descriptions below. */
void *thread1(void *);
void *thread2(void *);
void *thread3(void *);

/*
Thread 1 - User Command Interface:
User interface. Allows user to send old commands to PIC, print current
temperature from TMP102, set new temperature threshold, and as one option
disable/enable camera scanning mode or change camera scanning mode range
(180 to 0 OR 90 to 0).
Idea: ScanningMode90 or ScanningMode180 or SCANNING_OFF

Thread 2 - Sensor Control Center:
Sends commands and receives data from LDR, TMP102, and Camera. In loop, if
temperature over threshold, take picture; and if light intensity changed
dramatically, change camera setup or variables.

Thread 3 - Client-Server Communication
Every two seconds, send data to server by making an HTTP POST request of the following format:
http://server_hostname:portnumber/update? id=var_xxxx&password=var_xxxx&name=var_xxxx&data=var_xxxx&status=var_xxxx&tim estamp=var_xxxx&filename=var_xxxx.
id = 10; b(int) because we are group ten.
password = "password"; the default
name = "Kyle"; one student's name
status = "Online" | "Error"; status of PIC
data = [10-bit int from PIC ADC]
timestamp = "2017-11-09_09:12:34"; whatever the fucking date is
filename = "image02.jpg"(or whatever) | "No face detected"
*/


#endif

