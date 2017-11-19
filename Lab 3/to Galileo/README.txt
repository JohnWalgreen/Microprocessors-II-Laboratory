Header files:
	i2c.h
		- declares functions for communicating and receiving data from
			the temperature sensor
		- defines macros: ADDRESS, ADAPTER_NUMBER, and NUM_SAMPLES
	pic.h
		- declares function for taking picture
		- defines macros: DEST_FOLDER and PICTURE_LIMIT

Source Files:
	i2c.c
		- defines functions declared in "i2c.h"
	pic.c
		- defines functions declared in "pic.h"
	main.c
		- defines main function to interact with user, poll temperature,
			and take pictures
