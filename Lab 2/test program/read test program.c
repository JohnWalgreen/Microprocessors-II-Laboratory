/*Reads from GPIO ports.*/

#define HIGH 0b1
#define LOW 0b0
#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

/*FILE SPECIFIC DEFINES*/
#define ON 1000
#define OFF 200
#define PAUSE 1000

int read();
void GPIO_Init();
void interrupt ISR();

void main() {

	GPIO_Init();

	TRISAbits.TRISA0 = 0;
	ANSELAbits.ANSA0 = 0;

	while (1) {
		//LATAbits.LATA0 = PORTBbits.RB5 == HIGH ? HIGH : LOW;
	}
	return;
}

int read() {
	return (PORTB >> 1) & 0xF;
}

/*NOT REAL*/
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