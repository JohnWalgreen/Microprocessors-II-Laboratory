#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

#define _XTAL_FREQ 500000            //500khz speed for delay in microseconds
#define LED_THRESHOLD 125
#define LED_ROLLOVER 1500
#define PWM_ROLLOVER 1500

    
// ++++++++++++ Helpful Notes ++++++++++++++++


/*
 include or set any library or definition you think you will need
 */

// ====================  prototype functions: ====================

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
 
 void me1() {
 	// p. 286 setup??????
 	// p.287 pwmxcon
 	
 	//1 ?
 	T2CLKCON.CS = 0b0100;//2
 	
 	//3
 	T2HLT.PSYNC = 0; // t2psync not snchronized
 	T2HLT.MODE = 0b00000; // t2mode software control (period pulse, software gate, p.442 and p.424)
 	T2HLT.CKPOL = 0;
 	T2HLT.CKSYNC = 0;
 	// timer mode?
 	
 	// 4
 	T2RST.RSEL = 0b00001;   // p.443 (ccp1?)
 	//T2CON.CKPS = T2CKIPPS pin?
 	// reset source? i think done with rsel
 	
 	// 5
 	PR2 = 255; // maybe?
 	
 	// 6 ?
 	
 	/// 7
 	PIR1bitsTMR2IF = 0;
 	PIR4.TRM2IF = 0;			//?
 	
 	// 8
 	
 	
 }
 void Timer2_Init(void)
{

    PR2 = 255;                      // Preset PR2. Changing the value of PR2 changes the period and the cycle.
    T2CONbits.T2CKPS = 16;          // TMR2 Prescale = 16;
    PIR4bits.TMR2IF = 0x00;         // Clear "IF" flag
    T2CON = 0b00000111;             // Activate T2CON.
    T2CONbits.T2ON = 1;             // Turn Timer 2 on
}



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
  void PWM_Init(void)
{

    PR2 = 0b10011011;               // Set PR2 to ~155
    CCP1CON = 0b00111100;           // Set CCP1 to PWM output
    CCPR1H = 0;                     // CCPR register set to 0.
    CCPR1L = 0;
    CCPTMRS0bits.C1TSEL = 0b01;      // Select timer 2 for PWM CCP1 Module
}


/*  example: void ADC_Init(void)  {}   
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
 * 12. Set ADC result alignment, Enable ADC module, Clock Selection Bit, Disable ADC Continuous Operation, Keep ADC inactive
 
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
    ADREFbits.ADPREF1 = 0;     //V references

    ADSTATbits.ADSTAT0 = 1;
    ADSTATbits.ADSTAT1 = 1;    //ADC conversion module
    ADSTATbits.ADSTAT2 = 0;

    ADPCHbits.ADPCH0 = 1;
    ADPCHbits.ADPCH1 = 0;       //ADC positive channel select
    ADPCHbits.ADPCH2 = 0;
    ADPCHbits.ADPCH3 = 0;
    ADPCHbits.ADPCH4 = 0;
    ADPCHbits.ADPCH5 = 0;
}



/*
 PWM_signal_out() prototype function 
 *
 *- you set 10bits value for the duty cycle being careful with the MSB/LSB alignment 
 *- Set the appropriate Registers in the right sequence
 */

/* PWN_signal_out_X()
 Will write the PWM signal to determine either the left or the right directions,
 based on the duty cycle instructed by the CCPR1L and CCP1CON registers.

 To use: Call PWM_signal_out_LEFT() to move the servo to the left by changing the duty cycle to 10%,
 Call PWN_signal_out_RIGHT() to move the servo to the right by changing the duty cycle to 10%.
 A duty cycle of ~15% would call for a middle-aligned servo.
 *
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

/*
 ADC_conversion_results() prototype function
 * 
 * - set your ADC channel , activate the ADC module , and get the ADC result to a value , then deactivate again the ADC module
 * - Set the appropriate Registers in the right sequence
 */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++




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
    ADC_Init();
    PWM_Init();
    
    OSCCON1 = 0x38;            // 500khz clock speed
    //__CONFIG(FOSC_INTOSC);          // Clock source is the internal oscillator.
    
	int led_counter;
    long pwm_counter;
    int direction;
    
    led_counter = 0;
    pwm_counter = 0;
    direction = 0;

    // Initialize the required modules

    // set up conversion parameters

	TRISAbits.TRISA0 = 0;			// TRISC bit0 is output
	TRISAbits.TRISA1 = 1;			// PORTC bit1 is input
	ANSELAbits.ANSA1 = 1;				// bit 1 is analog input
    // TRISCAbits.TRISCA[something] = 0;   // output    HOW???????????????

    while (1) // keep your application in a loop
    {
        
        /*START PWM SECTION*/
        if (pwm_counter >= PWM_ROLLOVER) {
            direction = !direction;
            if (direction == 0) {
                PWM_signal_out_LEFT();
            } else {
                PWM_signal_out_RIGHT();
            }
            pwm_counter = 0;
        } else {
            pwm_counter++;
        }
        /*END PWM SECTION*/
        
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
}

/**
 End of File
*/
