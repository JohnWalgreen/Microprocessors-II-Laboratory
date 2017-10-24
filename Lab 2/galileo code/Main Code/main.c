#include "gpio.h"
#include 

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

		input &= 0xF;		// get last 4 bits (this line is technically unnecessary

		// place data on bus
		data [0] = openGPIO(GP_4, GPIO_DIRECTION_OUT);
		
	}
}