/*CURRENTLY WORKING ON THIS - BE PATIENT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/*
INFORMATION

ORDER OF CONTENTS
	1) Note
	2) Pin Layout
	3) Writing procedure
	4) Reading procedure
	5) Safety note to avoid damaging devices
	6) Idea
	7) GPIO Strobe Signal Implementation Uncertainty

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

7) GPIO Strobe Signal Implementation Uncertainty
The GPIO signal seems to always be low when not writing, so how does the PIC know 
when a read procedure has begun?  Two ideas to solve this problem: the two-pin method 
and the toggle method.  The two-pin method involves each piece of hardware outputting 
a GPIO signal used to indicate when to write.  The toggle method involves 
enabling/disabling interrupts costantly; when write is done, GPIO stays high and when 
write is done, GPIO stays low.  Interrupts are triggered by GPIO signal changes.
*/

/*ADC Constants*/
#define LED_ROLLOVER 1000			// counter value, which triggers adc conversion to update LED
#define LED_THRESHOLD 150			// adc threshold, which determines the value required to enable LED

/*PWM Constants*/
// Kyle - help me out here
// LEFT? RIGHT? CENTER?

/*GPIO Constants*/
// 4-bit instructions from computer
#define MSG_RESET 0x0 // reset the sensor to initial state
#define MSG_PING 0x1 // check if the sensor is working properly
#define MSG_GET 0x2 // obtain the most recent ADC result
#define MSG_TURN30 0x3 // turn the servo motor blade 30 degrees
#define MSG_TURN90 0x4 // turn the servo motor blade 90 degrees
#define MSG_TURN120 0x5 // turn the servo motor blade 120 degrees
// 4-bit responses from PIC
#define MSG_ACK 0xE // acknowledgement to the commands
#define MSG_NOTHING 0xF // reserved

/*Include files and other shit here*/

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
void interrupt();	// change name of this function, you fucking dumbass
// read, execute, and respond (write) accordingly

void main() {

	// initialise system
	System_Init();		// is this function right?
	ADC_LED_Init();
	PWM_Init();
	GPIO_Init();
	
	// initialise other variables such as counters and other crap
	
	while (1) {
	
		/*LED stuff*/
		// from old code
		
		/*Check for and send messages?*/
		/*
		pseudo-code:
			if strobe=low
				write msg_ack	[if high, interrupt is triggered]
		*/
		
	} // end infinite loop
	
	return 0;
	
} // end main
