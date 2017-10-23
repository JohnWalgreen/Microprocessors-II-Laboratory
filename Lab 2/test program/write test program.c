#define HIGH 0b1
#define LOW 0b0
#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

void GPIO_Init();
void interrupt ISR();

void main() {

	GPIO_Init();

	TRISB = 0xFF;
	TRISB &= 0xE1;

	while (1) { continue;}

	return;
}

void write(int instruction) {
	LATB &= 0xE1;
	LATB |= (instruction & 0xF) << 1;
}

void GPIO_Init() {

	TRISB = 0xFF;
	ANSELB = 0x00;

	PIE0bits.IOCIE = 1;
	// PIE0bits.INTE = 1; // I don't need this line, so I fucking got rid of it
	IOCBPbits.IOCBP5 = 1;               // enable positive-edge on-change interrupt for RB5, which will always be digital input
	INTCONbits.PEIE = 1;                // I think this is enable peripherel interrupts?
	INTCONbits.GIE = 1;                 // enable global interrupts
} // end of GPIO_Init

void interrupt ISR() {
	write(LATB + 1);
	return;
}