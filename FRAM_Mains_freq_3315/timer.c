#include <msp430.h>
#include "timer.h"
void TIMER_B_initCapture()
{
TB0CTL = TASSEL1 | ID0 | ID1 | MC0 | TAIE; 			// taif is here
TB0CCTL1 = CM0   | CAP | CCIE | CCIS0; 				//COV is here ccif is here  P1.4
TBCCR0 = 0xFFFF;
}

void TIMER_A1_initSampler()			//same at 800hz 1.25ms
{
	TA1CCR1 = 19900;				// adc start sampling pulse at
	TA1CCR0 = 19968;				// count upto
	TA1CTL = TASSEL1  | MC0  ; 			// main clock is 8Mhz or 16Mhz
	TA1CCTL1 = OUTMOD0 | OUTMOD1;
}

void TIMER_A0_initScreenUpdater()
{
TA0EX0= 0x7;								//further div by8 500Khz clock 262.14ms
TA0CCR0 = 0xFFFF;
TA0CTL = TASSEL1 | ID0 | ID1 | MC0 | TAIE; 	// taif is here
}

