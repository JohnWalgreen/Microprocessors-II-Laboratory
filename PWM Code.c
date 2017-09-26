
#include "mcc_generated_files/mcc.h" //default library 
#define _XTAL_FREQ 500000            //500khz speed for delay in microseconds.


// ++++++++++++ Helpful Notes ++++++++++++++++


/*
 include or set any library or definition you think you will need
 */

// ====================  prototype functions: ====================


/* There are two ways to generate a PWM signal: 
 *  
   1 . Using the internal PWM module ( enabling and configuring CCP1 module and enabling Timer 2 )  - If you go with this way there is a guide below to enable Timer 2 and CCP1 module
   2 . Handling a GPIO pin and delay functions  - Come p with your own prototype funciton
 *
*/


/*  Enabling and Configuring Timer 2 : Optional
 * 
 *  example: void Timer2_Init(void)  {}     
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
void Timer2_Init(void)
{
    __CONFIG(FOSC_INTOSC);          // Clock source is the internal oscillator.
    OSCCON = 0b00111000;            // 500khz clock speed
    PR2 = 255;                      // Preset PR2. Changing the value of PR2 changes the period and the cycle. 
    T2CONbits.T2CKPS = 16;          // TMR2 Prescale = 16;
    PIR4bits.TMR2IF = 0x00;         // Clear "IF" flag
    T2CON = 0b00000111;             // Activate T2CON.
    T2CONbits.T2ON = 1;             // Turn Timer 2 on
}


/*   Enabling and Configuring CCP1 module : Optional
 * 
 * example: void PWM_Init(void)  {}  
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
    CCPTMRSbits.C1TSEL = 0b00;      // Select timer 2 for PWM CCP1 Module
}

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


/*  Enabling and configuring ADC module:  Mandatory  
 * 
 * example: void ADC_Init(void)  {}   
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

void main(void)
{
    // Initialize PIC device
    SYSTEM_Initialize();

    // Initialize the required modules 
   
    // **** write your code 
    
    

    while (1) // keep your application in a loop
    {
        
        
        // ****** write your code 
        
        
        
        // Debug your application code using the following statement
        printf("Hello Lab 1 \n\r");
        
        
        
    }
    
    
}

/**
 End of File
*/
