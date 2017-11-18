#include "thread_functions.h"
#include <stdlib.h>     // defines NULL
#include <pthread.h>
#include <semaphore.h>

/*
Every thread takes one void * argument.  This argument will be casted to a
(Data *).  In this strcuture exists variables that must be updated and tools for
synchronisation.
*/

// thread 1
void *interface(void *arg) {
	
	/*
	Requirements:
		print user command options
		read user choice
		print appropriate response
		
	Options include:
		1) Reset PIC
		2) Ping PIC
		3) GET PIC ADC value
		4) Turn 30
		5) Turn 90
		6) Turn 120
		7) Get temperature
		8) Set new temperature threshold
		9) Scan 90 degree radius
		10) Scan 180 desgree radius
		11) Disable scanning
		
	Below is a description of what every option must accomplish.  Whenever a
	statement is followed by "protect" in paranthesis, the directive must be
	protected via snchronisation.
	Ex. "ping PIC (protect)" means ping the PIC but use the pic_mux and pic_sem
	to make sure that communication is clean.
	
	Option 1, 2, 3, 4, 5, 6, 9, 10, 11:
		Send command to PIC (proect), update value in Data (protect) if
		applicable, and print message to user.
		
	Option 7:
		Communicate with TMP102 and read temperature (protect), update values
		inside Data (protect) if applicable, and print temperature.

	Option 8:
		Prompt for new threshold and update value in Data (protect).

	TO DO (on outside):
		1) add synchronisation variables for I2C inside Data.
		2) add commands for scanning on Galileo and PIC side
	*/
	
	Data *data;
	
	pthread_exit(NULL);
	return NULL;
}

void *sensor_control(void *arg) {
	/*
	Requirements:
		talk to all sensors and update values in Data
		tae picture if necessary
	*/
}
void *client_server_communication(void *arg) {
	/*
	Requirements:
		send data to server every two seconds
	*/
}
