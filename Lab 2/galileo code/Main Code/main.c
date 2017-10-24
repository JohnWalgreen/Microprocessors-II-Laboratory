#include "gpio.h"

void write(int data, int *bus) {
	writeGPIO(bus[0], value & 0x1);
	writeGPIO(bus[1], (value >> 1) & 0x1);
	writeGPIO(bus[2], (value >> 2) & 0x1);
	writeGPIO(bus[3], (value >> 3) & 0x1);
}

int read(int *bus) {
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
	int flag;

	int strobe;
	int data [4];			// 4 pins

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

			} else if (input < 0 || input > 5) {
				puts("Error: %d is an invalid option\n", input);
				flag = -1;
			}

		} while (flag);

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

		// 2
		write(input & 0xF, data);

		// 3
		writeGPIO(strobe, HIGH);

		//4
		sleep(0.01);

		
	}
}