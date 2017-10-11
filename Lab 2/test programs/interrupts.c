/*RA1 will be GPIO signal which connects to RB0
Triggers interrupt when set to high
Every interrupt RA0 will be toggled
connect RA0 to lED for ouput
*/


// other
#define DEBOUNCE_DELAY 3        // amount to delay during iterrupt in order to debounce

/*Include files and other shit here*/
#define HIGH 0b1
#define LOW 0b0
#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

#define LED_ROLLOVER 100000

/*Initialise GPIO communication w/ computer*/
void GPIO_Init();
// set up interrupt for messages

/*Interrupt function for messages from computer*/
void interrupt ISR();
// read, execute, and respond (write) accordingly

void main() {

	// initialise system w/ given functions
	SYSTEM_Initialize();
	OSCILLATOR_Initialize();

	// finish initialising with our own functions
	GPIO_Init();

	// declare other variables such as counters and other crap
	int led_counter;

	// initialisations
	led_counter = 0;

	while (1) {
		switch (led_counter) {
			case 0:
				LATAbits.LATA1 = HIGH;
				break;

			case 10:
				LATAbits.LATA1 = LOW;
				break;

			case LED_ROLLOVER:
				led_counter = -1;
				break;

		}

		led_counter++;

	}


	return; // it is a void function

} // end main



void GPIO_Init() {
	/*
	1) GIE bit of INTCON
	2) Interrupt enable bits for specific event
	3) PEIE bit of INTCON register if #2 inside a PIEx register

	GIE bit must be set after every interrupt.

	What I found at http://www.microcontrollerboard.com/pic_interrupt.html
	INTF=0; // Reset the external interrupt flag
	INTE=1; // Enable external interrupts from RB0
	*/


	IOCBPbits.IOCBP0 = 1;         	// interrupt on positive-edge change for RB0 (the GPIO strobe signal input); p.261
	PIE0bits.IOCIE = 1;             // enables <<interrupt-on-change>>; p. 133
	// INTCONbits.PEIE = 1;         // enable all peripherel active interrupts; p. 132 - edit: according to p.133 note, this is not necessary
	// INTCONbits.INTEDG = 1;      	// rising edge if INT interrupt; applicable?
	INTCONbits.GIE = 1;             // enable all active interrupts

	TRISB = 0xFF;                  // everything in PORTB is a digital input
	TRISAbits.TRISA0 = 0;           // LED output
	TRISAbits.TRISA1 = 0;           // trigger interrupt - connect to port b0
} // end of GPIO_Init

void interrupt ISR() {

	// check source
	if (PIR0bits.IOCIF == HIGH && IOCBFbits.IOCBF0 == HIGH) {
		// check if <<Interrupt-on-Change Interrupt Flag bit (read-only); p. 142>> is HIGH
		// indicates that interrupt-on-change caused interrupt
		// check if RB0 was the on-change interrupt (p. 261)
		// by design, only positive-edge change is enabled

		// debounce
		__delay_ms(DEBOUNCE_DELAY);
		if (PORTBbits.RB0 == HIGH) {
			// interrupt is legit, so handle it, dumbass

			LATAbits.LATA0 = (LATAbits.LATA0 == LOW);   // flip
		}

	}   // else if other flags to determine other sources of interrupt

	// re-enable interrupts
	INTCONbits.GIE = 1;
}
