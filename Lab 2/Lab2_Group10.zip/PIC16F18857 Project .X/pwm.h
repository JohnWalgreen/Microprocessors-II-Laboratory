#ifndef PWM_H
#define	PWM_H

/*Initialise PWM*/
void PWM_Init();    // Kyle will be doing this with counters so just initialise outputs on proper pins

/*Move senso-motor (via PWM signals)*/
void PWM_Turn30();
void PWM_Turn90();
void PWM_Turn120();

#endif	/* PWM_H */

