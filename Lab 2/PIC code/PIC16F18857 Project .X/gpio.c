#include "gpio.h"
#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

void GPIO_Init() {
	communication_counter = 0;
	
	TRISB = 0xFF;
	ANSELB = 0x00;						// PORT B is digital input

	PIE0bits.IOCIE = 1;
	// PIE0bits.INTE = 1; // I don't need this line, so I fucking got rid of it
	IOCBPbits.IOCBP5 = 1;               // enable positive-edge on-change interrupt for RB5, which will always be digital input
	IOCBNbits.IOCBN5 = 1;               // enable negative-edge
	INTCONbits.PEIE = 1;                // I think this is enable peripherel interrupts?
	INTCONbits.GIE = 1;                 // enable global interrupts
} // end of GPIO_Init

int read() {
	return (PORTB >> 1) & 0xF;
}

void write(int instruction) {
	LATB &= 0xE1;
	LATB |= (instruction & 0xF) << 1;
}