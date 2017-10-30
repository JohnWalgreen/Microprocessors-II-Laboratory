#include "pwm.h"
#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

void PWM_Init()
{
	TRISAbits.TRISA2 = 0; 	// TRISC pin 2 is output.
							// Will control the PWM of the servo motor.
	ANSELAbits.ANSA2 = LOW;
	LATAbits.LATA2 = 0;
};
void PWM_Turn30()
{
	int i;
	for (i = 0; i<15; i++) {
		LATAbits.LATA2 = 1; 	// Set PWM Signal HIGH for 1.16 ms, for a 30* angle.
		__delay_ms(0.9);
		LATAbits.LATA2 = 0;
		__delay_ms(18.2);
	}
};
void PWM_Turn90()
{
	int i;
	for (i = 0; i<15; i++) {
		LATAbits.LATA2 = 1; 	// Set PWM Signal HIGH for 1.16 ms, for a 30* angle.
		__delay_ms(1.5);
		LATAbits.LATA2 = 0;
		__delay_ms(18.5);
	}
};
void PWM_Turn120()
{
	int i;
	for (i = 0; i<15; i++) {
		LATAbits.LATA2 = 1; 	// Set PWM Signal HIGH for 1.16 ms, for a 30* angle.
		__delay_ms(2.1);
		LATAbits.LATA2 = 0;
		__delay_ms(17.9);
	}
};
