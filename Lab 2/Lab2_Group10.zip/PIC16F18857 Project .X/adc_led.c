#include "adc_led.h"
#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

void ADC_LED_Init() {
	// from <<main.c>> in Lab 1
	/*
	(Comment from Lab 1 template)
	* 1. Set ADC CONTROL REGISTER 1 to 0
	* 2. Set ADC CONTROL REGISTER 2 to 0
	* 3. Set ADC THRESHOLD REGISTER to 0
	* 4. Disable ADC auto conversion trigger control register
	* 5. Disable ADACT
	* 6. Clear ADAOV ACC or ADERR not Overflowed  related register
	* 7. Disable ADC Capacitors
	* 8. Set ADC Precharge time control to 0
	* 9. Set ADC Clock
	* 10 Set ADC positive and negative references
	* 11. ADC channel - Analog Input
	* 12. Set ADC result alignment, Enable ADC module, Clock Selection Bit, Disable ADC Continuous Operation, Keep ADC inactive.

	+ set up pins
	*/

	adc_value = 0;          // latest adc value

	ADCON1 = 1;
	ADCON2 = 0;
	ADCON3 = 0;				// set adc threshold reg to 0
	ADACT = 0;				// disable adc auto conversion trigger (is this how????????????)
						// 5 already disabled, genius
	ADSTATbits.ADAOV = 0;	// I hope this works?
	ADCAP = 0;		// 7
	ADPRE = 0;		// 8
	ADCON0bits.ADFM = 1;	// right justifed!

	ADCON0bits.ADON = 1;	// adc enable
	ADCON0bits.ADCS = 0b101;// Clock supplied by FOSC, divided according to ADCLK register? (0) vs. Clock supplied from FRC dedicated oscillator
	ADCON0bits.ADCONT = 0;	// disable continuous operation

	ADREFbits.ADNREF = 0;
	ADREFbits.ADPREF0 = 0;
	ADREFbits.ADPREF1 = 0;	//V references

	ADSTATbits.ADSTAT0 = 1;
	ADSTATbits.ADSTAT1 = 1;	//ADC conversion module
	ADSTATbits.ADSTAT2 = 0;

	ADPCHbits.ADPCH0 = 1;
	ADPCHbits.ADPCH1 = 0;	//ADC positive channel select
	ADPCHbits.ADPCH2 = 0;
	ADPCHbits.ADPCH3 = 0;
	ADPCHbits.ADPCH4 = 0;
	ADPCHbits.ADPCH5 = 0;

	// Set up the required pins for the function, inputs and outputs.
	TRISAbits.TRISA0 = 0;			// RA0 is LED output
	TRISAbits.TRISA1 = 1;			// RA1 is input from light sensor
	ANSELAbits.ANSA1 = 1;			// RA1 input is analog
} // end of ADC_LED_Init
