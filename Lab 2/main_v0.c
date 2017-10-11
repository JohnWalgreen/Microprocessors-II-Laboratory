/*
File Created On: 09-Oct-2017
Release Date (successful compile and run): TBD
Preceding File: <<my-skeleton.c>>
Succeeding File: TBD

Description:
	This file is the first main program for Lab 2.
This program will be designed to match the specifications
layed out in the <<provided materials>> folder, and this
program will most likely resemble the skeleton code in
<<my-skeleton.c>>.

Edits: (format= Name: [name of coder making changes]\nDate: [Today's Date or date range]\nTime: [time range]\nDescription: [description of all changes]\n\n)

Example of Entry!!!
Name: Joe Schmo
Date: 04-July-1977
Time: 0200 - 1523
Description: ... everything that was accomplished ...

Name: Hans-Edward Hoene
Date: 09-Oct-2017
Time: 1520 - 1555
Description: File created from skeleton; <<ADC_LED_Init>>
function coded from Lab 1; LED and ADC code in infinite
while loop copied from Lab 1 main code.

Name: Hans-Edward Hoene
Date: 10-Oct-2017
Time: 1050 - 1315
Description: 	1) included files from Lab 1 main.c
		2) learned how the communication GPIO strobe signal is
			supposed to work.  Updated comments by removing
			<<7) GPIO Strobe Signal Implementation Uncertainty>>
			from the order of contents in the large opening comment.
		3) added in order of contents in large comment:
			<<7) How to implement GPIO stobe signal>>
		4) Added global variable, <<int last_ADC_value>> to keep
			track of the latest ADC vaue that was read for the LED.
		5) Coded GPIO_Init function; basically enables interrupts and
			sets inputs and outputs
		6) Changed name of interrupt function and coded it
		7) Added <<bits>> to end of register names
		8) Added DEBOUNCE_DELAY for on-change debouncing
		9) added HIGH and LOW constants
		10) added and coded read and write functions
			TO DO: Finish handling commands in switch-statement inside interrupt function
			TO DO: make adc right justified, but put entire 10-bit in int and always update global variable

Name: Hans-Edward Hoene
Date: 11-Oct-2017
Time: 1030-1050 & 1230 - 1500 & 1715 - 1748
Description: Worked on testing interrupt-on-change (in test interrupt.c program).  I have not been able to automatically 
trigger interrupts, but I have been able to manually start an interrupt by setting an interrupt flag.  Still same TO DOs 
as yesterday, and fix interrupt iniitialisations.  Also tried to get PWM stuff together.
*/

/*
INFORMATION

ORDER OF CONTENTS
	1) Note
	2) Pin Layout
	3) Writing procedure
	4) Reading procedure
	5) Safety note to avoid damaging devices
	6) Idea
	7) How to implement GPIO stobe signal

1) Note: When I say <<computer>>, I am referring to Intel
Galileo board running Linux Yocto.  And when I say <<PIC>>,
I am referring to PIC microcontroller, <<PIC16F18857>>.

2) Pins:
	PORTA:
		0: LED output
		1: ADC (light-sesnor) input
		2: PWM signal output
		3:
		4:
		5:
		6:
		7:

	PORTB:
		0: GPIO strobe input from computer
		1: GPIO data 0
		2: GPIO data 1
		3: GPIO data 2
		4: GPIO data 3
		5:
		6:
		7:

3) Writing Procedure (from computer to PIC)
	a) computer outputs 4-bit command
	b) computer raises GPIO strobe signal to HIGH, which triggers interrupt on the PIC
	c) the PIC reads what is being output; the output will only exist for 10ms so the PIC better fucking read it
	d) computer lowers strobe signal to low
	e) computer stops putting command on bus

4) Reading Procedure (from PIC to computer)
	a) computer pulls signal low
	b) PIC outputs 4-bit value (either MSG_ACK for fucking good or some of the god-damned ADC value)
	c) computer raises strobe signal to tell PIC that reading has begun
	d) computer puts strobe signal low again when done
	e) PIC stops outputting value

5) Safety Note to avoid damaging either device
The GPIO data bus pins should always be input on both devices (high impedance)
until the device is without doubt going to output data.  The concern in that
if both connected pins are low impedance and trying to write data, short circuits
may occur.  We cannot let this happen!
The GPIO strobe pin will always be output for the computer and input for the PIC.

6) Idea
One idea for if we are feeling like going above and beyond to to add
instructions a queue and remove them and execute in the main loop.
I have taken a look though, abnd I am not sure if this is possible
because some insyrcutions require immediate responses.

7) How to implement GPIO stobe signal
The PIC will not be sending messages unless it is respnding to an instruction.
When the GPIO signal goes high, an interrupt is triggered and a read operation is
performed.  The PIC processes the command and responds the correct number of times
before the PIC continues normal operation again.
*/

/*ADC Constants*/
#define LED_ROLLOVER 1000			// counter value, which triggers adc conversion to update LED
#define LED_THRESHOLD 150			// adc threshold, which determines the value required to enable LED

/*PWM Constants*/
// Kyle - help me out here
// LEFT? RIGHT? CENTER?

/*GPIO Constants*/
// 4-bit instructions from computer
#define MSG_RESET 0x0			// reset the sensor to initial state
#define MSG_PING 0x1			// check if the sensor is working properly
#define MSG_GET 0x2				// obtain the most recent ADC result
#define MSG_TURN30 0x3			// turn the servo motor blade 30 degrees
#define MSG_TURN90 0x4			// turn the servo motor blade 90 degrees
#define MSG_TURN120 0x5			// turn the servo motor blade 120 degrees
// 4-bit responses from PIC
#define MSG_ACK 0xE				// acknowledgement to the commands
#define MSG_NOTHING 0xF			// reserved
// other
#define DEBOUNCE_DELAY 3        // amount to delay during iterrupt in order to debounce

/*Include files and other shit here*/
#define HIGH 0b1
#define LOW 0b0
#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

/*Add Global Variables Here*/
int last_ADC_value;

/*Initialise ADC*/
void ADC_LED_Init();    // also make LED port an output

/*Initialise PWM*/
void PWM_Init();    // Kyle will be doing this with counters so just initialise outputs on proper pins

/*Initialise GPIO communication w/ computer*/
void GPIO_Init();
// set up interrupt for messages

/*Move senso-motor (via PWM signals)*/
void movePWM(unsigned int dest, unsigned int source);
// senso-motor might move if you are not currently generating
// PWM signals, so in implementation, you may need to always enable
// and disable PWM.  You can maybe do this by plugging the PWM vin to a
// port in the PIC, which will turn on and off.
// THINK THIS THROUGH! We don't have timer thing (we are allegedly using
// delays), so we can't be constantly generating PWM signals.  We must
// find a way to keep the damn senso-motor still when we are not moving it.
// As a side note, this thing must be quick.

/*Interrupt function for messages from computer*/
void interrupt ISR();
// read, execute, and respond (write) accordingly

int read();                 // set GPIO pins to inputs and read their value

void write(int);            // set GPIO pins to outputs and write value

void main() {

	// initialise system w/ given functions
	SYSTEM_Initialize();
	OSCILLATOR_Initialize();
	
	// finish initialising with our own functions
	ADC_LED_Init();
	PWM_Init();
	GPIO_Init();

	// declare other variables such as counters and other crap
	int led_counter;

	// initialisations
	led_counter = 0;

	while (1) {

		/*START LIGHT SENSOR AND LED PART*/
		/*
		Read me:
		For every iteration...

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
			if (ADCON0bits.GO == LOW) {
				// conversion = done

				// update led
				if (ADRESH < LED_THRESHOLD) {
					// light is high, photoresistor is low, analog input voltage is low, LED goes off
					LATAbits.LATA0 = LOW;
				} else {
					LATAbits.LATA0 = HIGH;
				}

				led_counter = 0;        // start counter over again
			}
		} else if (led_counter >= LED_ROLLOVER) {
			// it is time to start an adc conversion
			ADCON0bits.GO = LOW;   // start conversion
			led_counter = -1;   // indicates that conversion is running
		} else {
			// neither so just update counter
			++led_counter;
		}
		/*END LIGHT SENSOR AND LED PART*/

	} // end infinite loop

	return; // it is a void function

} // end main

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

	ADCON1 = 1;
	ADCON2 = 0;
	ADCON3 = 0;				// set adc threshold reg to 0
	ADACT = 0;				// disable adc auto conversion trigger (is this how????????????)
						// 5 already disabled, genius
	ADSTATbits.ADAOV = 0;	// I hope this works?
	ADCAP = 0;		// 7
	ADPRE = 0;		// 8
	ADCON0bits.ADFM = 0;	// left justified alignment?

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

void GPIO_Init() {
	/*
	1) GIE bit of INTCON
	2) Interrupt enable bits for specific event
	3) PEIE bit of INTCON register if #2 inside a PIEx register

	GIE bit must be set after every interrupt.

	What I found at http://www.microcontrollerboard.com/pic_interrupt.html
	INTF=0; // Reset the external interrupt flag
	INTE=1; // Enable external interrupts from RB0
	*/

	IOCBPbits.IOCBP0 = 1;         	// interrupt on positive-edge change for RB0 (the GPIO strobe signal input); p.261
	PIE0bits.IOCIE = 1;             // enables <<interrupt-on-change>>; p. 133
	// INTCONbits.PEIE = 1;         // enable all peripherel active interrupts; p. 132 - edit: according to p.133 note, this is not necessary
	// INTCONbits.INTEDG = 1;      	// rising edge if INT interrupt; applicable?
	INTCONbits.GIE = 1;             // enable all active interrupts

	TRISB = 0xFF;                  // everything in PORTB is a digital input
} // end of GPIO_Init

void interrupt ISR() {

	// check source
	if (PIR0bits.IOCIF == HIGH && IOCBFbits.IOCBF0 == HIGH) {
		// check if <<Interrupt-on-Change Interrupt Flag bit (read-only); p. 142>> is HIGH
		// indicates that interrupt-on-change caused interrupt
		// check if RB0 was the on-change interrupt (p. 261)
		// by design, only positive-edge change is enabled

		// debounce
		__delay_ms(DEBOUNCE_DELAY);
		if (PORTBbits.RB0 == HIGH) {
			// interrupt is legit, so handle it, dumbass
			
			int instruction;
			
			/*HANDLE COMMUNICATION HERE*/
			// read command, which should already be being outputted
   			instruction = read();
			
			// wait for GPIO to go low again
			do {
				while (PORTBbits.RB0 == HIGH) {continue;}       // wait for low
				__delay_ms(DEBOUNCE_DELAY);                 	// debounce
			} while (PORTBbits.RB0 == HIGH);
			
			// READING OVER
			switch (instruction) {
				case MSG_RESET:
					// reset sensor?
					break;
				case MSG_PING:
					write(MSG_ACK);
					break;
				case MSG_GET:
					/* write last adc result (3 times) !!!!!!!!!!!!!!!!!!!!*/
					write(MSG_ACK);
					break;
				case MSG_TURN30:
					/*KYLE'S FUNCTIONS*/
					break;
				case MSG_TURN90:
					/*KYLE'S FUNCTIONS*/
					break;
				case MSG_TURN120:
					/*KYLE'S FUNCTIONS*/
					break;		
			}
			
			// wait for GPIO to go high again
			do {
				while (PORTBbits.RB0 == LOW) {continue;}       // wait for high
				__delay_ms(DEBOUNCE_DELAY);                 // debounce
			} while (PORTBbits.RB0 == LOW);
			
			// wait for GPIO to go low again
			do {
				while (PORTBbits.RB0 == HIGH) {continue;}       // wait for low
				__delay_ms(DEBOUNCE_DELAY);                 // debounce
			} while (PORTBbits.RB0 == HIGH);
			
			PORTB = PORTB | 0x78;   // back to inputs
		}

	}   // else if other flags to determine other sources of interrupt

	// re-enable interrupts
	INTCONbits.GIE = 1;
}

void PWM_Init() {
    // CODE THIS, KYLE!!!
}

int read() {
	int instruction;
	PORTB = PORTB | 0x78;       // set up inputs just in case
	
	instruction = 0;
	if (PORTBbits.RB1 == HIGH) {
		instruction += 1;
	}
	if (PORTBbits.RB2 == HIGH) {
		instruction += 2;
	}
	if (PORTBbits.RB3 == HIGH) {
		instruction += 4;
	}
	if (PORTBbits.RB4 == HIGH) {
		instruction += 8;
	}
	
	return instruction;
}

void write(int instruction) {
	PORTB = PORTB & 0x87;   // set up outputs
	
	if (instruction > 8) {
		LATBbits.LATB4 = HIGH;
		instruction -= 8;
	} else {
		LATBbits.LATB4 = LOW;
	}
	
	if (instruction > 4) {
		LATBbits.LATB3 = HIGH;
		instruction -= 4;
	} else {
		LATBbits.LATB3 = LOW;
	}
	
	if (instruction > 2) {
		LATBbits.LATB2 = HIGH;
		instruction -= 2;
	} else {
		LATBbits.LATB2 = LOW;
	}
	
	if (instruction > 1) {
		LATBbits.LATB1 = HIGH;
		instruction -= 1;
	} else {
		LATBbits.LATB1 = LOW;
	}
}
