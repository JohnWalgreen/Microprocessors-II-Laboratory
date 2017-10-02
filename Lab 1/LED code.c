
#include "mcc_generated_files/mcc.h" //default library 
#include <htc.h>

/*
 ADC_conversion_results() prototype function
 * 
 * - set your ADC channel , activate the ADC module , and get the ADC result to a value , then deactivate again the ADC module
 * - Set the appropriate Registers in the right sequence
 */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define LED_THRESHOLD 125
#define LED_ROLLOVER 1500

/*
 * Read Me -- Hans
 * Light-sensor/LED controller
 
 * <<led_counter>> holds an integer.  Every infinite loop iteration, led_counter will increment.
 * Once it is equal to LED_ROLLOVER (defined above), led_counter will be set to -1 and the adc conversion will begin.
 * While it is -1, check to see if conversion is done.  If so, set led_counter back to 0 and allow it to increment again, and update the led
 * This method, although complex, saves interrupts for more important things, and it saves clock cycles.
 
 */


/*
Develop your Application logic below
*/

void main(void)
{
    // Initialize PIC device
    SYSTEM_Initialize();
    
    int led_counter;
    led_counter = 0;

    // Initialize the required modules 
   
    // set up conversion parameters
    
	TRISAbits.TRISA0 = 0;			// TRISC bit0 is output
	TRISAbits.TRISA1 = 1;			// PORTC bit1 is input
	ANSELAbits.ANSA1 = 1;				// bit 1 is analog input
    
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
    
    /*
    //LATAbits.LATA0 = 1;
    __delay_ms(1000);
    //LATAbits.LATA0 = 0;
    
    while(1) {
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO != 0) {continue;}
    
        //if (ADRESH == 0) {
        //    LATAbits.LATA0 = 1;
        //} else {                    //Testing ADRESH register constantly zero
         //   LATAbits.LATA0 = 0;
        //}
        LATAbits.LATA0 = 0;
        led_counter = ADRESH;
        led_counter /= 10;
        while (led_counter != 0) {
            LATAbits.LATA0 = 1;
            __delay_ms(500);
            LATAbits.LATA0 = 0;
            __delay_ms(500);
            led_counter--;
        }
        
        LATAbits.LATA0 = 1;
        __delay_ms(3000);
    
    }*/
    
    

    while (1) // keep your application in a loop
    {
        
        //START LED/LIGHT SENSOR PART
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
        //END LED/LIGHT SENSOR PART
        
        
        
        
    }
    
    
}
