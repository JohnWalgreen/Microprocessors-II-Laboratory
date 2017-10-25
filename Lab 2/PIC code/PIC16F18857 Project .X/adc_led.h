#ifndef ADC_LED_H
#define	ADC_LED_H

/*ADC Constants*/
#define LED_ROLLOVER 10			// counter value, which triggers adc conversion to update LED
// high threshold = hard to turn on, low threshold = easy to turn on
#define LED_THRESHOLD 350    // adc value must go below this to turn on
#define OFFSET 20

int adc_value;      // global variable
                    // most recent adc value
int max, min, threshold;

void ADC_LED_Init();    // also make LED port an output

#endif	/* ADC_LED_H */
