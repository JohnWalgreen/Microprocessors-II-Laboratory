/*Strobe (40) pin 8
GP_4 LSB and GP_7 MSB for data bus (A0-A3)
*/

#include "gpio.h"

// Warning: is sleep_ms defined? use <<usleep(unsigned int)>> in unistd.h for microseconds

void writeBus(int value, int *bus) {
	writeGPIO(bus[0], value & 0x1);
	writeGPIO(bus[1], (value >> 1) & 0x1);
	writeGPIO(bus[2], (value >> 2) & 0x1);
	writeGPIO(bus[3], (value >> 3) & 0x1);
}

int readBus(int *bus) {
	// readGPIO must return 0 or 1 !!!
	int value = 0;
	value |= (readGPIO(bus[0]));
	value |= (readGPIO(bus[1]) << 1);
	value |= (readGPIO(bus[2]) << 2);
	value |= (readGPIO(bus[3]) << 3);
	return value;
}

int main() {

	int input;			// user command input
	int response;		// response from PIC

	int flag;

	int strobe;				// handle for strobe signal
	int data [4];			// handles for data bus pins (A3-A0)

	strobe = openGPIO(Strobe, GPIO_DIRECTION_OUT);	// always out!
	writeGPIO(strobe, LOW);

	while (1) {

		puts("Options");
		puts("0) Reset");
		puts("1) Ping");
		puts("2) Get ADC value");
		puts("3) Turn Senso-motor to 30 degrees");
		puts("4) Turn Senso-motor to 90 degrees");
		puts("5) Turn Senso-motor to 120 degrees\n");
		puts("-1) Exit program");

		do {
			printf("Enter command: ");
			flag = 1 - scanf("%d", &input);

			if (flag) {
				char gahbage;

				puts("ERROR: Invalid number input\n");

				// must flush line!
				do {
					scanf("%c", &gahbage);
				} while (gahbage != '\n');

			} else if (input < -1 || input > 5) {			// remember that -1 is valid input
				printf("Error: %d is an invalid option\n", input);
				flag = -1;
			}

		} while (flag);

		// exit if input = -1
		if (input < 0) {
			closeGPIO(Strobe, strobe);
			return 0;
		}

		/*
		START STEP 1
			1) open all pins as outputs
			2) put data on bus
			3) flip strobe on
			4) Give pic 10ms to read data
		*/

		// 1
		data[0] = openGPIO(GP_4, GPIO_DIRECTION_OUT);
		data[1] = openGPIO(GP_5, GPIO_DIRECTION_OUT);
		data[2] = openGPIO(GP_6, GPIO_DIRECTION_OUT);
		data[3] = openGPIO(GP_7, GPIO_DIRECTION_OUT);

		writeBus(input & 0xF, data);		// 2
		writeGPIO(strobe, HIGH);		// 3
		usleep(10000);					// 4


		/*END STEP 1*/

		/*STEP 2 -- read data from PIC*/
		flag = 0;
		response = 0;
		while ((flag < 4 && input == MSG_GET) || flag < 1) {
			// if msg_get, read 4 times
			// else, just read response

			/*
			READ FROM PIC
			1) bring strobe low
			2) remove data from bus
			3) make pins inputs after closing them
			4) give PIC some auxiliary some extra time to generate response
			5) raise strobe high
			6) give PIC time to convert pins from inputs to outputs
			7) read bus
			*/

			writeGPIO(Strobe, LOW);				// 1
			writeBus(0, data);						// 2

			// 3
			closeGPIO(GP_4, data[0]);
			closeGPIO(GP_5, data[1]);
			closeGPIO(GP_6, data[2]);
			closeGPIO(GP_7, data[3]);
			data[0] = openGPIO(GP_4, GPIO_DIRECTION_IN);
			data[1] = openGPIO(GP_5, GPIO_DIRECTION_IN);
			data[2] = openGPIO(GP_6, GPIO_DIRECTION_IN);
			data[3] = openGPIO(GP_7, GPIO_DIRECTION_IN);

			usleep(2000);						// 4
			writeGPIO(Strobe, HIGH);			// 5
			usleep(2000);						// 6
			response += (readBus(data) << flag);	// 7 + extra

			++flag;

		}
		/*END STEP 2*/

		/*START STEP 3 -- just switch strobe to low to indicate that communication is over, and close pins*/
		writeGPIO(Strobe, LOW);
		closeGPIO(GP_4, data[0]);
		closeGPIO(GP_5, data[1]);
		closeGPIO(GP_6, data[2]);
		closeGPIO(GP_7, data[3]);
		/*END STEP 3*/

		// return status message
		if ((response & 0xF) == MSG_ACK) {
			// good response code
			switch (input) {
				case MSG_RESET:
					puts("PIC successfully reset");
					break;
				case MSG_PING:
					puts("PIC has returned ping");
					break;
				case MSG_GET:
					response = (response >> 4);			// last 4 bits is MSG_ACK, upper 10 bits is data
					printf("Last ADC value: %d\n", response);
					break;
				case MSG_TURN30:
					puts("PIC has queued command to turn senso-motor to 30 degrees");
					break;
				case MSG_TURN90:
					puts("PIC has queued command to turn senso-motor to 90 degrees");
					break;
				case MSG_TURN120:
					puts("PIC has queued command to turn senso-motor to 120 degrees");
					break;
			}
		} else {
			// if last 4 bits of response (the response code) are bad
			puts("An unexpected error ocurred.");
		}
		puts("\n\n");
		
	}

	return 0;
}
