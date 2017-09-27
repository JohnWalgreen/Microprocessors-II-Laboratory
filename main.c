#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

#define _XTAL_FREQ 500000           //500khz speed for delay in microseconds
#define LED_THRESHOLD 100
#define LED_ROLLOVER 1000
#define PWM_ROLLOVER 11000

#define LEFT 0                      // Adjust the duty cycles for the PWM LEFT module
#define CENTER 7                    // Adjust the duty cycles for the PWM CENTER module
#define RIGHT 15                    // Adjust the duty cycles for the PWM RIGHT module


    
// ++++++++++++ Helpful Notes ++++++++++++++++


/*
 include or set any library or definition you think you will need
 */

// ====================  prototype functions: ====================  //

/* =========================== TIMER PROTOTYPES ===============================*/

/*  example: void Timer2_Init(void)  {}     
 *  Configure Timer 2 and start it
    
 USE Timer 2
 * 
 * ---- Set the Registers below:-----------
 * 
 * 1. Set the Basic Configuration Register for Timer 2 
 * 2. Set the Clock Source in relation with system Oscillator frequency 
 * 3. T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not Synchronized; Timer Mode
 * 4. T2RSEL T2CKIPPS pin; Reset Source
 * 5. Set PR2 255; 
 * 6. Set TMR2  Prescale Value to 0 
 * 7. Clearing IF flag.
 * 8. Start Timer2
 * 
 * ----------------------
 */

/* CURRENTLY UNUSED TIMER MODULE
 void Timer2_Init(void)
{
    //RC2PPS = 0x09;
    T2HLTbits.PSYNC = 0b0;            // PSYNC Not Synchronized
    T2HLTbits.MODE = 0b00000;             // T2MODE Software Controlled
    T2HLTbits.CKPOL = 0b0;            // T2CKPOL Rising Edge
    T2HLTbits.CKSYNC = 0b0;           // T2CKSYNC Not Synchronized
    T2RSTbits.RSEL = 0b10001;       // See page 443
    PR2 = 255;                      // Preset PR2. Changing the value of PR2 changes the period and the cycle.
    T2CONbits.T2CKPS = 0b0000;      // TMR2 Prescale = 16;
    T2CLKCONbits.T2CS = 0b0001;
    PIR4bits.TMR2IF = 0b0;         // Clear "IF" flag
    T2CONbits.TMR2ON = 1;             // Turn Timer 2 on
    
}
*/

/*================================== PWM INITIATE =======================================*/

/*  example: void PWM_Init(void)  {}  
 *  Configure CCP1 module in PWM mode (PWM channel = RC3 by default, Timer selection)
  Registers:
 * 
 * --- Set the Registers below: ---
 * 
 * 1. Set up CCP1 module register in PWM mode and bits alignment
 * 2. Set RH to 0; 
 * 3. Set RL to 0; 
 * 4. Selecting Timer 2 - for PWM in CCP1 module
 * 
 * ---------------
  */
 
/* CURRENTLY UNUSED PWM MODULE 
void PWM_Init(void)
{
    CCP1PPS = 0;
    CCP1CON = 0b10001111;           // Set CCP1 to PWM output
    PR2 =  255; //0b10011011;               // Set PR2 to ~155
    CCPR1H = 0;                     // CCPR register set to 0.
    CCPR1L = 0;
    CCPTMRS0bits.C1TSEL = 0b01;     // Select timer 2 for PWM CCP1 Module
    CCP1CONbits.EN = 1;
    TRISCbits.TRISC2 = 0;
}*/

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

/* ============================= PWM OUTPUT MODULES ============================*/

/*
 PWM_signal_out() prototype function 
 *
 *- you set 10bits value for the duty cycle being careful with the MSB/LSB alignment 
 *- Set the appropriate Registers in the right sequence
 */

  
void PWM_signal_out_LEFT() // 10% duty cycle, set CCPR1L and CCP1CON registers.
{
    CCPR1L = 0b00001111;
    CCP1CON = 0b00101100;
}


void PWM_signal_out_RIGHT() // 20% duty cycle, set CCPR1L and CCP1CON registers.
{
    CCPR1L = 0b00011111;
    CCP1CON = 0b00001100;
}

/* ========================== MAIN PROGRAM CODING ==============================*/

/*
Develop your Application logic below
*/

/*
 * Read Me -- Hans
 * Light-sensor/LED controller
 * <<led_counter>> holds an integer.  Every infinite loop iteration, led_counter will increment.
 * Once it is equal to LED_ROLLOVER (defined above), led_counter will be set to -1 and the adc conversion will begin.
 * While it is -1, check to see if conversion is done.  If so, set led_counter back to 0 and allow it to increment again, and update the led
 * This method, although complex, saves interrupts for more important things, and it saves clock cycles.
 */

void main(void)
{
    // Initialize PIC device
    SYSTEM_Initialize();
    OSCCON1 = 0xFF;
    
    // Initialize the required modules
    ADC_Init();
    
    // Timer2_Init();                       // UNUSED TIMER MODULE, SAVED FOR POTENTIAL USE LATER
    // PWM_Init();                          // UNUSED PWM MODULE, SAVED FOR POTENTIAL USE LATER
    
    // Initialize the required variables for the function of the circuit.
    int led_counter;
    long pwm_counter;
    int direction;
    
    led_counter = 0;
    pwm_counter = 0;
    direction = 0;


    // Set up the required pins for the function, inputs and outputs.

	TRISAbits.TRISA0 = 0;			// TRISC bit0 is output
	TRISAbits.TRISA1 = 1;			// PORTC bit1 is input
	ANSELAbits.ANSA1 = 1;			// bit 1 is analog input
    TRISAbits.TRISA3 = 0;
    


int direction;
int duty_counter;

direction = LEFT;
duty_counter = pwm_counter = 0;

/*START TEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

direction = RIGHT;
    

/*END TEST!!!!!!!!!!!!!!!!!!!*/


while (1) {
if (pwm_counter >= PWM_ROLLOVER) {
		// change direction
		switch (direction) {
		case LEFT:
			direction = RIGHT;
			break;
		default:
			direction = LEFT;
		}

		pwm_counter = 0;
	} else {

		/*switch (direction) {
			case LEFT:
				if (TRISAbits.TRISA3 == 1 && duty_counter == 1) {
					TRISAbits.TRISA3 = 0;
					duty_counter = 0;
				} else {
				}
		}*/


		if (LATAbits.LATA3 == 0b1) {	// if off
            // keep LED on for direction duty cycles
			if (duty_counter >= direction) {
				LATAbits.LATA3 = 0b0;
			}
            
            duty_counter++;
            
		} else {	// if off
            // keep LED off for remainder of cycles (everything out of 100)
			if (duty_counter >= 100) {
				LATAbits.LATA3 = 0b1;
				duty_counter = 0;
			} else {
				duty_counter++;
			}
		}
        pwm_counter++;
	}
        /*START LIGHT SENSOR AND LED PART*/
        if (led_counter < 0) {
            // adc conversion is running - check if done
            if (ADCON0bits.GO == 0) {
                // conversion = done

                // update led
                if (ADRESH < LED_THRESHOLD) {
                    LATAbits.LATA0 = 0;
                } else {
                    LATAbits.LATA0 = 1;
                }

                led_counter = 0;        // start counter over again
            }   // else, just keep checking every iteration
        } else if (led_counter >= LED_ROLLOVER) {
            // it is time to start an adc conversion
            ADCON0bits.GO = 1;   // start conversion
            led_counter = -1;   // indicates that conversion is running
        } else {
            // neither so just update counter
            ++led_counter;
        }
        /*END LIGHT SENSOR AND LED PART*/
}
	/*
	READ ME
	
	If moving right, duty=20%
	Stay on for 1 cycle, turn off for 4 cycles
	
	Center
	on for 3 cycles, off for 17
	Left
	on for 1 cycles, off for 9
	*/

	if (pwm_counter >= PWM_ROLLOVER) {
		// change direction
		switch (direction) {
		case LEFT:
			direction = RIGHT;
			break;
		default:
			direction = LEFT;
		}

		pwm_counter = 0;
	} else {

		/*switch (direction) {
			case LEFT:
				if (TRISAbits.TRISA3 == 1 && duty_counter == 1) {
					TRISAbits.TRISA3 = 0;
					duty_counter = 0;
				} else {
				}
		}*/


		if (LATAbits.LATA3 == 0b1) {	// if off
            // keep LED on for direction duty cycles
			if (duty_counter >= direction) {
				LATAbits.LATA3 = 0b0;
			}
            
            duty_counter++;
            
		} else {	// if off
            // keep LED off for remainder of cycles (everything out of 100)
			if (duty_counter >= 100) {
				LATAbits.LATA3 = 0b1;
				duty_counter = 0;
			} else {
				duty_counter++;
			}
		}
        pwm_counter++;
	}

        
        /*START LIGHT SENSOR AND LED PART*/
        if (led_counter < 0) {
            // adc conversion is running - check if done
            if (ADCON0bits.GO == 0) {
                // conversion = done

                // update led
                if (ADRESH < LED_THRESHOLD) {
                    LATAbits.LATA0 = 0;
                } else {
                    LATAbits.LATA0 = 1;
                }

                led_counter = 0;        // start counter over again
            }   // else, just keep checking every iteration
        } else if (led_counter >= LED_ROLLOVER) {
            // it is time to start an adc conversion
            ADCON0bits.GO = 1;   // start conversion
            led_counter = -1;   // indicates that conversion is running
        } else {
            // neither so just update counter
            ++led_counter;
        }
        /*END LIGHT SENSOR AND LED PART*/

    
}

/**
 End of File
*/
