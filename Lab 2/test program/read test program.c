/*Reads from GPIO ports.*/

#define HIGH 0b1
#define LOW 0b0
#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

/*FILE SPECIFIC DEFINES*/
#define ON 10000
#define OFF 200
#define PAUSE 1000

int read();
void GPIO_Init();
void interrupt ISR();

void main() {

	GPIO_Init();

	TRISB = 0xFF;
	TRISAbits.TRISA0 = 0;
	ANSELAbits.ANSA0 = 0;

	while (1) {
		//LATAbits.LATA0 = PORTBbits.RB5 == HIGH ? HIGH : LOW;
	}
	return;
}

int read() {
	return PORTB >> 1;
}

/*NOT REAL*/
void GPIO_Init() {

	TRISBbits.TRISB5 = 1;				// RB5 is input
	ANSELBbits.ANSB5 = 0;				// RB5 is digital

	TRISBbits.TRISB1 = 1;
	TRISBbits.TRISB2 = 1;
	TRISBbits.TRISB3 = 1;
	TRISBbits.TRISB4 = 1;               // GPIO input set up

	PIE0bits.IOCIE = 1;
	// PIE0bits.INTE = 1; // I don't need this line, so I fucking got rid of it
	IOCBPbits.IOCBP5 = 1;               // enable positive-edge on-change interrupt for RB5, which will always be digital input
	INTCONbits.PEIE = 1;                // I think this is enable peripherel interrupts?
	INTCONbits.GIE = 1;                 // enable global interrupts
} // end of GPIO_Init

void interrupt ISR() {
	int val;
	val = 0xF & read();

	LATAbits.LATA0 = HIGH;
	if ((val >> 3) & 0x1) {
		__delay_ms(ON);
	} else {
		__delay_ms(OFF);
	}
	LATAbits.LATA0 = LOW;
	__delay_ms(PAUSE);

	LATAbits.LATA0 = HIGH;
	if ((val >> 2) & 0x1) {
		__delay_ms(ON);
	} else {
		__delay_ms(OFF);
	}
	LATAbits.LATA0 = LOW;
	__delay_ms(PAUSE);

	LATAbits.LATA0 = HIGH;
	if ((val >> 1) & 0x1) {
		__delay_ms(ON);
	} else {
		__delay_ms(OFF);
	}
	LATAbits.LATA0 = LOW;
	__delay_ms(PAUSE);

	LATAbits.LATA0 = HIGH;
	if (val & 0x1) {
		__delay_ms(ON);
	} else {
		__delay_ms(OFF);
	}
	LATAbits.LATA0 = LOW;

	IOCBFbits.IOCBF5 = LOW;     // clear flag
}