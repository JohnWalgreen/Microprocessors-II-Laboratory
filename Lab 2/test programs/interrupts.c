/*RA7 will be GPIO signal which connects to RA6
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

void interrupt ISR() {

	//if (PIR0bits.IOCIF == HIGH && IOCBFbits.IOCBF0 == HIGH) {
		// check if <<Interrupt-on-Change Interrupt Flag bit (read-only); p. 142>> is HIGH
		// indicates that interrupt-on-change caused interrupt
		// check if RB0 was the on-change interrupt (p. 261)
		// by design, only positive-edge change is enabled

		// debounce
		//__delay_ms(DEBOUNCE_DELAY);
		//if (PORTBbits.RB0 == HIGH) {
			// interrupt is legit, so handle it, dumbass

            // toggle
            if (LATAbits.LATA0 == LOW) {
                LATAbits.LATA0 = HIGH;
            } else {
                LATAbits.LATA0 = LOW;
            }
        //}


	//}   // else if other flags to determine other sources of interrupt

	
    IOCAF = 0;

    return;
}
// read, execute, and respond (write) accordingly

void main() {

	// initialise system w/ given functions
	SYSTEM_Initialize();
	OSCILLATOR_Initialize();
	
	/*START GPIO_Init FUNCTION*/
	TRISAbits.TRISA6 = 1;                  // everything in PORTB is a digital input
	TRISAbits.TRISA0 = 0;           // LED output
	TRISAbits.TRISA7 = 0;           // trigger interrupt - connect to port c0
	PIE0bits.IOCIE = 1;
	PIE0bits.INTE = 1; // do I need this?
	IOCAPbits.IOCAP6 = 1;
	INTCONbits.PEIE = 1;
	INTCONbits.GIE = 1;
	/*END GPIO_Init FUNCTION*/
	
	// declare other variables such as counters and other crap
	int counter = 0;
	while (1) {
		switch (counter) {
			case 0:
				LATAbits.LATA7 = HIGH;
				if (PORTAbits.RA6 == HIGH)	// I just added this if-statement here to see 
								// if PIC even knows that RA6 is on -
								// that could be the problem
								// without this, I know LED flashes. Will it still work?
					IOCAFbits.IOCAF6 = 1;   // ARTIFICIALLY GENERATE INTERRUPT
								// it should be doing it on its own tbh, but it isn't.
								// who the fuck knows why?
				break;
				
			case 10:
				LATAbits.LATA7 = LOW;
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
