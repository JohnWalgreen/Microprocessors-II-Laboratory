/*
Goal:
set up an interrupt on change for RB0

Reality:
RB0 will be input for GPIO strobe signal,
which indicates that a communication is
about to start.  When a positive edge
arises, the interrupt will happen and the
communication will proceed.

To test:
RA7 will be an output that is plugged into
RB0 (input).  RA6 wil be the test GPIO
signal.  RA6 will programmatically turn on
and off.  Every positive edge, an interrupt
shall occur and the RA0 output will toggle
(connected to LED).

NOTE: we could not get square wave out, but
interrupts seem functional.
*/


// other
#define DEBOUNCE_DELAY 5        // amount to delay during iterrupt in order to debounce

/*Include files and other shit here*/
#define HIGH 0b1
#define LOW 0b0
#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

void GPIO_Init();

void interrupt ISR();
// read, execute, and respond (write) accordingly

void main() {

	// initialise system w/ given functions
	SYSTEM_Initialize();
	OSCILLATOR_Initialize();
	
	TRISAbits.TRISA0 = 0;           // LED output
	TRISCbits.TRISC6 = 0;           // trigger interrupt - connect to port c0
	ANSELCbits.ANSC6 = 0;

	/*START GPIO_Init FUNCTION*/
	GPIO_Init();
	/*END GPIO_Init FUNCTION*/

	// declare other variables such as counters and other crap
	int counter = 0;
	while (1) {
		switch (counter) {
			case 0:
				LATCbits.LATC6 = HIGH;

				break;

			case 50:
				LATCbits.LATC6 = LOW;
				break;

			case 100:
				counter = -1;
				break;
		}

		counter++;
		__delay_ms(10);
	}

	return; // it is a void function
} // end main

void GPIO_Init() {
	TRISBbits.TRISB0 = 1;                  // everything in PORTB is a digital input
	ANSELBbits.ANSB0 = 0;              // everything in PORTB is digital input

	PIE0bits.IOCIE = 1;
	//PIE0bits.INTE = 1; // I don't need this line, so I fucking got rid of it
	IOCBPbits.IOCBP0 = 1;
	INTCONbits.PEIE = 1;
	INTCONbits.GIE = 1;
}

void interrupt ISR() {

	if (PIR0bits.IOCIF == HIGH && IOCBFbits.IOCBF0 == HIGH) {
		// check if <<Interrupt-on-Change Interrupt Flag bit (read-only); p. 142>> is HIGH
		// indicates that interrupt-on-change caused interrupt
		// check if RB0 was the on-change interrupt (p. 261)
		// by design, only positive-edge change is enabled

		// debounce
		__delay_ms(DEBOUNCE_DELAY);
		if (PORTBbits.RB0 == HIGH) {
			// interrupt is legit, so handle it, dumbass
			
			// toggle
			if (LATAbits.LATA0 == LOW) {
				LATAbits.LATA0 = HIGH;
			} else {
				LATAbits.LATA0 = LOW;
			}
		}
		
		IOCBFbits.IOCBF0 = LOW;     // clear flag
	}   // else if other flags to determine other sources of interrupt
	return;
}
