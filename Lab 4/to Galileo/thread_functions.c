#include "thread_functions.h"
#include <stdlib.h>     // defines NULL

/*
Every thread takes one void * argument.  This argument will be casted to a
(Data *).  In this strcuture exists variables that must be updated and tools for
synchronisation.
*/

// thread 1
void *interface(void *) {
	
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
		
	Option 1:
		Send reset command to PIC (protect), and use response to update
		Data->status (protect) and print success message.
		
	Option 2:
		Ping PIC (protect), and use response to update Data->status (protect),
		and print success message.

	Option 3:
		Send GET command to PIC (protect), and use response to update
		Data->adc_value (protect) and print adc value.

	Option 4, 5, 6:
		Send appropriate turn command to PIC (protect), and use response to
		update Data->status (protect) and print message.

	Option 7:
		Read temperature from I2C device (protected), change Data temperature
		(protected), and [add new semaphore for this], and print.

	Option 8:
		Prompt for and change Data temp threshold.

	Option 9, 10, 11:
		Send commands to PIC which update scanning variables.
	*/
	
	return NULL;
}

void *sensor_control(void *);   				// thread 2
void *client_server_communication(void *);    	// thread 3
