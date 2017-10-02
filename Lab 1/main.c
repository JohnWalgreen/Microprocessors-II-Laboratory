#include "mcc_generated_files/mcc.h" //default library

// ++++++++++++ Helpful Notes ++++++++++++++++


/*
 include or set any library or definition you think you will need
 */

#include <htc.h>

#define _XTAL_FREQ 500000           //500khz speed for delay in microseconds

#define LED_THRESHOLD 100	    // high threshold = hard to turn on, low threshold = easy to turn on
#define LED_ROLLOVER 1000
#define PWM_ROLLOVER 11000

#define LEFT 0                      // Adjust the duty cycles for the PWM LEFT module
#define CENTER 7                    // Adjust the duty cycles for the PWM CENTER module
#define RIGHT 15                    // Adjust the duty cycles for the PWM RIGHT module

// ====================  prototype functions: ====================  //

/*======================== ADC INITIATE MODULE ==================================*/
/* void ADC_Init(void) module
 * Purpose: Initializes the ADC registers to be prepared for an analog to digital conversion.
 * Inputs: None 
 * Outputs: Changes the registers listed below.
 *    
 *  Configure ADC module  
 ----- Set the Registers below::
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
 * 
  */ 
  
  void ADC_Init(void) {
  	//ADCON0 = 0;
    ADCON1 = 1;
    ADCON2 = 0;
    ADCON3 = 0;                     // set adc threshold reg to 0
    ADACT = 0;                      // disable adc auto conversion trigger (is this how????????????)
                                    // 5 already disabled, genius
    ADSTATbits.ADAOV = 0;                      // I hope this works?
    ADCAP = 0;                      // 7
    ADPRE = 0;                      // 8

    ADCON0bits.ADFM = 0;			// left justified alignment?
    // maybe ^ = ADCON0bits.ADFRM0 = 0???
    ADCON0bits.ADON = 1;                // adc enable
    ADCON0bits.ADCS = 0b101;                //  Clock supplied by FOSC, divided according to ADCLK register? (0) vs. Clock supplied from FRC dedicated oscillator
    ADCON0bits.ADCONT = 0;              // disable continuous operation

    //ADNREF = 0;                 //negative voltage reference: Vss
    //ADPREF1 = 0;                //positive voltage reference: Vdd
    //ADPREF0 = 0;                //positive voltage reference

    //ADCON0 = ADCON0 | 0x44;
    //ADCON0 = ADCON0 & 0XE7;

    //ADCON0bits.VCFG = 0;
    ADREFbits.ADNREF = 0;
    ADREFbits.ADPREF0 = 0;
    ADREFbits.ADPREF1 = 0;          //V references
    
    ADSTATbits.ADSTAT0 = 1;
    ADSTATbits.ADSTAT1 = 1;         //ADC conversion module
    ADSTATbits.ADSTAT2 = 0;

    ADPCHbits.ADPCH0 = 1;
    ADPCHbits.ADPCH1 = 0;           //ADC positive channel select
    ADPCHbits.ADPCH2 = 0;
    ADPCHbits.ADPCH3 = 0;
    ADPCHbits.ADPCH4 = 0;
    ADPCHbits.ADPCH5 = 0;
}

/* ========================== MAIN PROGRAM CODING ==============================*/

/*
Develop your Application logic below
*/

void main(void) {
	
	// Initialize PIC device
	SYSTEM_Initialize();
	OSCCON1 = 0xFF;
	
	// Initialize the required modules
	ADC_Init();
	
	// Initialize the required variables for the function of the circuit.
	int led_counter;        // when counter reaches LED_ROLLOVER, update LED based on light sensor input
	long pwm_counter;       // when counter reaches PWM_ROLLOVER, change direction by changing duty cycle
	int direction;          // the direction the PWM motor is moving
	int duty_counter;       // counter used to flip RA3 output on and off to match duty cycle
	
	led_counter = 0;
	pwm_counter = 0;
	direction = LEFT;
	duty_counter = 0;

    	// Set up the required pins for the function, inputs and outputs.
	TRISAbits.TRISA0 = 0;			// RA0 is LED output
	TRISAbits.TRISA1 = 1;			// RA1 is input from light sensor
	ANSELAbits.ANSA1 = 1;			// RA1 input is analog
	TRISAbits.TRISA3 = 0;           	// RA3 is PWM motor output
	
	// while loop has a pwm section and a light sensor section

	while (1) {
		
		/*START PWM SECTION*/
		
		/*
		Read me:
		
		Direction is changed when <<pwm_counter>> rolls over.  Output
		on/off square wave is managed by <<duty_counter>>.
		
		<<pwm_counter>> will increment every loop.  When it reaches
		<<PWM_ROLLOVER>> (pre-defined constant), PWM motor will change
		direction.  The <<direction>> variable holds the duty cycle
		that the PWM is supposed to obtain.  The duty cycle for
		aspired directions is contained in pre-defined constants:
		<<LEFT>>, <<RIGHT>>, <<CENTER>>.
		
		Whenever the direction is not going to be changed,
		<<duty_counter>> will increment.  Until <<duty_counter>> reaches
		the variable held in <<direction>>, which is just the duty cycle
		as a percent, the RA3 PWM output will remain HIGH.  Once
		<<duty_counter>> does reach <<direction>>, the RA3 output will be
		turned off until <<duty_counter>> reaches 100.  So in other words,
		the duty cycle is <<direction>> / 100.
		*/
		
		// check if you must change direction
		// note: pwm_counter updated at end
		if (pwm_counter >= PWM_ROLLOVER) {
			
			// change direction and reset counter
			switch (direction) {
				
				case LEFT:
					direction = RIGHT;
					break;
				
				default:
					direction = LEFT;
			}

			pwm_counter = 0;
		
		} else {
			
			// keep direction, manage duty cycle
			
			// check if RA3 is on
			if (LATAbits.LATA3 == 0b1) {
				
            	// turn off only when counter reaches direction, which is percentage of duty cycle
				if (duty_counter >= direction) {
					LATAbits.LATA3 = 0b0;
				}
            
            	++duty_counter; // always increment counter
            
			} else {
				
				// RA3 is off, turn on only when duty_counter reaches 100 (%) which means that the cycle is complete
				if (duty_counter >= 100) {
					// turn on and reset counter
					LATAbits.LATA3 = 0b1;
					duty_counter = 0;
				} else {
					// otherwise, increment counter so that it is out of 100 instead of 101
					// 0-99 before reset = 100
					duty_counter++;
				}
				
			}
			
		}
		
		++pwm_counter;      // update pwm counter
		/*END PWM MOTOR SECTION*/
		
		
        /*START LIGHT SENSOR AND LED PART*/
        
        /*
        Read me:
		For every iiteration...
		
		if <<led_counter>> is less than zero, a conversion
		is running.  Check if it is done.  If it is done,
		use results to update LED output and set counter
		back to zero.  Otherwise, just leave and check in
		the next iteration.
		
		When <<led_counter>> reaches LED_ROLLOVER (pre-defined),
		start the ADC conversion and set counter to a number
		less than zero.
		
		Otherwise, increment the counter.
		
		This method saves clock cycles by not waiting for conversions
		and by adc converting once once every rollover.  This method also saves interrupts for more important shit.
 		*/
 
        if (led_counter < 0) {
            // adc conversion is running
            
            // only if conversion is done,
			// update LED based on LED_THRESHOLD (pre-defined)
			// and reset counter
            if (ADCON0bits.GO == 0) {
                // conversion = done

                // update led
                if (ADRESH < LED_THRESHOLD) {
		    // light is high, photoresistor is low, analog input voltage is low, LED goes off
                    LATAbits.LATA0 = 0;
                } else {
                    LATAbits.LATA0 = 1;
                }

                led_counter = 0;        // start counter over again
            }
            
        } else if (led_counter >= LED_ROLLOVER) {
            // it is time to start an adc conversion
            
            ADCON0bits.GO = 1;   // start conversion
            led_counter = -1;   // indicates that conversion is running
            
        } else {
        	
            // neither so just update counter
            ++led_counter;
            
        }
        
        /*END LIGHT SENSOR AND LED PART*/
        
	}   // end infinite while loop
	
}       // end main

/**
 End of File
*/
