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

Name: Hans-Edward Hoene
Date: 13-Oct-2017
Time: 1020 - 1256
Description: 	1) modified interrupt test program and got it to work.  The problem might have been with ANSELx digital/analog
					input, but I am not sure.  I changed GPIO strobe interrupt code here to match what  test program has.
				2) Changed led latest value to adc_value global; changed adc to right justified to get full 10-bits
				3) Debounced ADC conversion by adding lower/upper thresholds for the LED enable.  Implemented via
					threshold+offset and threshold-offset.  Larger offsets allow for larger swings in light without LED
					flickers.

Name: Kyle Marescalchi
Date: 14-October-2017
Time: 19:00 - 20:00
Description: Implemented the required functions for the functions MSG_TURN[XXX], updated the required code for the 'PWM' delay output.

Name: Hans-Edward Hoene
Date: 14-Oct-2017
Time: 2210 - 2226
Description: Fixed indentation in files, and I removed interrupt handler code that has not been tested (all in a big comment).  Got rid of test 
files that have not been started.

Name: Hans-Edward Hoene
Date: 17-Oct-2017
Time: 1247 - 1319
Description:    ALL) Started working on communication.  Interrupt every time GPIO changes!
				1) Added negative-edge interrupt for GPIO RB0
				2) Added communication_counter global to handle what this particular interrupt is for
				3) Added instruction as global variable to maintain instruction in between interrupts
				4) ISSUE: debouncing properly involves distinguishing postive and negative interrupts
				5) Issue at bullet 4 fixed by looking at counter % 2
				6) write and read no longer change TRISB for GPIO pins
				7) FIX: write out proper values for adc in large switch statement
				
Name: Hans-Edward Hoene
Date: 20-Oct-2017
Time: 1030 - 1320 & night
Description:    1) Merge Kyle's PWM code with mine.
				2) fixed adc value 4-bit conversion for writing (used shift + and)
				3) initialise communication_counter global variable in gpio init
				4) TO DO:	- add more support for other instructions in execute
						- add file structure tree to help find what you are loking for
						- better comments for all prototypes
						- eliminate redundancy in PWM code
						- test
						- eliminate led_threshold and offset
							- instead, use min and max range variables that update.  They determine threshold.
						- is adc_value unsigned? it should be
						- average variable for adc? influence starts at 1
							- next influence = 1 / ((1 / influence) + 1)
							- next average = avg + influence * (new_value - avg)
						- can average vs. range/2 determine offset? Maybe offset is directly proportional to range?
						- does PWM need multiple square wave peaks? Does no signal move PWM to 0 degrees? If yes to last question, 
							how do we turn PWM on/off?
						- fix led +/-offset in code for proper debounce (< is fine)(< + offset is not fine)

Name: Hans-Edward Hoene
Date: 22-Oct-2017
Time: 1600 - ...
Description:	1) Made threshold dynamic
				2) Got rid of execute function and just put it in main
				3) Eliminated redundant PWM code
				4) Referenced ANSELA in PWM init to select digital output
				5) RA3 no longer needed to disable/enable PWM motor.
				6) Fixed communication error. I forgot to write back MSG_ACK when adc bits were done sending.

Name: Kyle Marescalchi
Date: 22-Oct-2017
Time: 1715 - 1750
Description:	1) Got PWM functios to work.
				2) Discovered that PWM requires +6V for power supply.
				3) Discovered that 0% duty cycle triggers no movement, so RA3 was no longer needed.
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
		0: BROKEN PORT!!!!!!! RIP - will be missed (shoutout to Derek)
		1: GPIO data 0
		2: GPIO data 1
		3: GPIO data 2
		4: GPIO data 3
		5: GPIO strobe input
		6: NEVER USE!
		7: NEVER USE!

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
#define LED_ROLLOVER 10			// counter value, which triggers adc conversion to update LED
// high threshold = hard to turn on, low threshold = easy to turn on
#define LED_THRESHOLD 350    // adc value must go below this to turn on
#define OFFSET 20

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
#define DEBOUNCE_DELAY 5        // amount to delay during iterrupt in order to debounce

/*Include files and other shit here*/
#define HIGH 0b1
#define LOW 0b0
#include "mcc_generated_files/mcc.h" //default library
#include <htc.h>

#define BUFFER 5

typedef struct {
	unsigned int vals [BUFFER + 1];

	unsigned int front;			// index of first item to be removed from queue
	unsigned int back;			// index - 1 is last item in queue
} Queue;

int isFull(Queue);
int isEmpty(Queue);
void enqueue(Queue, unsigned int);
unsigned dequeue(Queue);

/*Add Global Variables Here*/
int adc_value;
int communication_counter;
int instruction;
Queue execution_queue;

/*Initialise ADC*/
void ADC_LED_Init();    // also make LED port an output

/*Initialise PWM*/
void PWM_Init();    // Kyle will be doing this with counters so just initialise outputs on proper pins

/*Initialise GPIO communication w/ computer*/
void GPIO_Init();
// set up interrupt for messages

/*Move senso-motor (via PWM signals)*/

void PWM_Turn30();
void PWM_Turn90();
void PWM_Turn120();

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
	int max, min;
	int threshold;

	// initialisations
	led_counter = 0;
	max = 0;
	min = 1023;
	threshold = 0;


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
				adc_value = (ADRESH << 8) + ADRESL;
				if (adc_value > max) {
					max = adc_value;
					threshold = (max + min) >> 1;
				}
				if (adc_value < min) {
					min = adc_value;
					threshold = (max + min) >> 1;
				}
				
				// debounce
				if (LATAbits.LATA0 == HIGH) {
					/*LED is off; to turn on, adc_value must go below the lower offset*/
					if (adc_value < threshold - OFFSET) {
						LATAbits.LATA0 = LOW;
					}
				} else {
					if (adc_value > threshold + OFFSET) {
						LATAbits.LATA0 = HIGH;
					}
				}
				
				led_counter = 0;        // start counter over again
			}
		} else if (led_counter >= LED_ROLLOVER) {
			// it is time to start an adc conversion
			ADCON0bits.GO = HIGH;   // start conversion
			led_counter = -1;   // indicates that conversion is running
		} else {
			// neither so just update counter
			++led_counter;
		}
		/*END LIGHT SENSOR AND LED PART*/
		
		/*Queue execution*/
		if (!isEmpty(execution_queue)) {
			switch (dequeue(execution_queue)) {
				case MSG_RESET:
					min = 1023;
					max = 0;
					threshold = 0;
					break;
				case MSG_TURN30:
					PWM_Turn30();
					break;
				case MSG_TURN90:
					PWM_Turn90();
					break;
				case MSG_TURN120:
					PWM_Turn120();
					break;
			}
		}

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

void GPIO_Init() {
	communication_counter = 0;
	
	TRISBbits.TRISB5 = 1;				// RB5 is input
	ANSELBbits.ANSB5 = 0;				// RB5 is digital

	TRISBbits.TRISB1 = 1;
	TRISBbits.TRISB2 = 1;
	TRISBbits.TRISB3 = 1;
	TRISBbits.TRISB4 = 1;               // GPIO input set up

	PIE0bits.IOCIE = 1;
	// PIE0bits.INTE = 1; // I don't need this line, so I fucking got rid of it
	IOCBPbits.IOCBP5 = 1;               // enable positive-edge on-change interrupt for RB5, which will always be digital input
	IOCBNbits.IOCBN5 = 1;               // enable negative-edge
	INTCONbits.PEIE = 1;                // I think this is enable peripherel interrupts?
	INTCONbits.GIE = 1;                 // enable global interrupts
} // end of GPIO_Init

void interrupt ISR() {

	// check source
	if (PIR0bits.IOCIF == HIGH && IOCBFbits.IOCBF5 == HIGH) {
		// check if <<Interrupt-on-Change Interrupt Flag bit (read-only); p. 142>> is HIGH
		// indicates that interrupt-on-change caused interrupt
		// check if RB0 was the on-change interrupt (p. 261)
		// by design, only positive-edge change is enabled
		
		// debounce
		__delay_ms(DEBOUNCE_DELAY);
		if (PORTBbits.RB5 == (communication_counter % 2 ? LOW : HIGH)) {
			// interrupt is legit, so handle it, dumbass
			
			switch (communication_counter) {
				case 0:
					// first interrupt, read value from GPIO bus
					// GPIO bus pins should already be set as inputs
					instruction = read();
					++communication_counter;
					break;
				case 1:
					// computer is done outputting signal
					// start processing and set up outputs already
					if (instruction == MSG_GET) {
						write((adc_value >> 8) & 0x3);
					} else {
						enqueue(execution_queue, instruction);
						write(MSG_ACK);
					}
					TRISB &= 0x87;          // set up outputs
					++communication_counter;
					break;
				case 2:
					// computer raises signal
					// reading has begun
					++communication_counter;
					break;
				case 3:
					// computer done reading value
					if (instruction == MSG_GET) {
						// write bit again
						write(adc_value >> 4 & 0xF);
						++communication_counter;
					} else {
						PORTB |= 0x78;  // back to inputs (high impedance)
						communication_counter = 0;      // next edge will be new command
					}
					break;
				case 4:
					// only get will come this far
					// reading has begun
					++communication_counter;
					break;
				case 5:
					// computer done reading
					// write one more value!
					write(adc_value & 0xF);
					++communication_counter;
				case 6:
					// reading has begun
					++communication_counter;
					break;
				case 7:
					// reading done
					write(MSG_ACK);
					++communication_counter;
					break;
				case 8:
					++communication_counter;
					// reading
					break;
				case 9:
					// all done w/ everything
					PORTB |= 0x78;
					communication_counter = 0;      // next edge is new instruction
					break;
				default:
					// handle error
					communication_counter = 0;      // next edge is new instruction
					break;
			} // end of switch statement
			
		}

		IOCBFbits.IOCBF5 = LOW;     // clear flag

	}   // else if other flags to determine other sources of interrupt

}

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

int read() {
	int instruction;

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

int isFull(Queue data) {
	return (
		((data.back + 1) % (BUFFER + 1))
			== data.front);
}
int isEmpty(Queue data) {
	return (data.back == data.front);
}
void enqueue(Queue data, unsigned int item) {
	if (!isFull(data)) {
		data.vals[data.back] = item;
		data.back = (data.back + 1) % (BUFFER + 1);
	}
}
unsigned int dequeue(Queue data) {
	unsigned int ret;
	if (!isEmpty(data)) {
		ret = data.vals[data.front];
		data.front = (data.front + 1) % (BUFFER + 1);
	}
	return ret;
}