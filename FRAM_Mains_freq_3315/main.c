#include <msp430.h> 
#include "eusci_b0_spi_5959.h"
#include "nokia_3315.h"
#include "timer.h"
#include "comp_e.h"
#include "adc.h"
#include "fft_lib.h"
/*
 * main.c
 */
void printInt(uint8_t col,uint8_t row,uint32_t num);
static struct cap_data{
	uint32_t data;
	volatile uint16_t flush;
	uint8_t	flag;
	uint16_t cap_m_state;
}cap;

struct adc_sampler_state{
uint16_t bufferfull;
uint16_t bindex;
uint16_t adbuff[256];
uint8_t	 trigger;
}adc;



#define ABS(x)   ((x)>0?(x):-(x))
#define MAP(in,inMin,inMax,outMin,outMax) ((((in)-(inMin))*((outMax)-(outMin)))/((inMax)-(inMin))+(outMin))

#define SCALE_ATTENUATION (230.0/17800.0)   // 230VAC <=> 1.5VAC @ input without attenuation


uint8_t vc=0;
#pragma vector = TIMER0_A1_VECTOR		//timer a0 over flor make it trig ever 333ms update display here
__interrupt void screen_update(void)
{
	P3OUT |= BIT6;							//debugging stuff
	P4OUT ^= BIT6;							//debugging stuff
	if(vc==2){
		if(adc.bufferfull)
		{
			printV(calcTRMS(adc.adbuff),0,0);
			adc.bufferfull=0;
			adc.trigger=1;
		}
		vc=0;


	if(cap.flag==4)
    	{
   		volatile uint32_t temp;
    		temp = (cap.data>>3); //like into x500 ) /100 and , x.5 is like div/2 , more 3 time to achive (125*2*2*2);
    		//we got up e-6
    		temp = 100000000/temp;
    		printHz(temp,0,4);

    		cap.flag = 0;
    		temp = cap.flush;
    		cap.data=0;
    		TB0CCTL1 &= ~(CCIFG |COV);
    		TB0CCTL1 |=CCIE;
    	}else
    	{
    		printHz(0,0,4);

    	}



}vc++;
TA0CTL &= ~TAIFG;
P3OUT &= ~BIT6;							//debugging stuff
}

/*
#pragma vector = TIMER1_A1_VECTOR		//ADC sampler timer ISR is not required as samling triggered by TA1CCR1 output which is internally connected
__interrupt void sampler_ISR(void)
{
 P1OUT ^= BIT2;
	TA1CTL &= ~TAIFG;
}
*/

#pragma vector = ADC12_VECTOR
__interrupt void adc_ISR(void)
{
	P2OUT ^= BIT5;									//debugging stuff
	adc.adbuff[adc.bindex+0]=ADC12MEM0;				/* copy adc results to buffer */
	adc.adbuff[adc.bindex+1]=ADC12MEM1;
	adc.adbuff[adc.bindex+2]=ADC12MEM2;				/* using for loop will save a little code space but add few extra cpu cycles to ISR which is not recomended*/
	adc.adbuff[adc.bindex+3]=ADC12MEM3;				//this copy section need to be replaced with DMA in final code
	adc.adbuff[adc.bindex+4]=ADC12MEM4;
	adc.adbuff[adc.bindex+5]=ADC12MEM5;
	adc.adbuff[adc.bindex+6]=ADC12MEM6;
	adc.adbuff[adc.bindex+7]=ADC12MEM7;
	adc.adbuff[adc.bindex+8]=ADC12MEM8;
	adc.adbuff[adc.bindex+9]=ADC12MEM9;
	adc.adbuff[adc.bindex+10]=ADC12MEM10;
	adc.adbuff[adc.bindex+11]=ADC12MEM11;
	adc.adbuff[adc.bindex+12]=ADC12MEM12;
	adc.adbuff[adc.bindex+13]=ADC12MEM13;
	adc.adbuff[adc.bindex+14]=ADC12MEM14;
	adc.adbuff[adc.bindex+15]=ADC12MEM15;
	adc.adbuff[adc.bindex+16]=ADC12MEM16;
	adc.adbuff[adc.bindex+17]=ADC12MEM17;
	adc.adbuff[adc.bindex+18]=ADC12MEM18;
	adc.adbuff[adc.bindex+19]=ADC12MEM19;
	adc.adbuff[adc.bindex+20]=ADC12MEM20;
	adc.adbuff[adc.bindex+21]=ADC12MEM21;
	adc.adbuff[adc.bindex+22]=ADC12MEM22;
	adc.adbuff[adc.bindex+23]=ADC12MEM23;
	adc.adbuff[adc.bindex+24]=ADC12MEM24;
	adc.adbuff[adc.bindex+25]=ADC12MEM25;
	adc.adbuff[adc.bindex+26]=ADC12MEM26;
	adc.adbuff[adc.bindex+27]=ADC12MEM27;
	adc.adbuff[adc.bindex+28]=ADC12MEM28;
	adc.adbuff[adc.bindex+29]=ADC12MEM29;
	adc.adbuff[adc.bindex+30]=ADC12MEM30;
	adc.adbuff[adc.bindex+31]=ADC12MEM31; //no need for ADC12IFGR1 &= ~ADC12IFG31; as reading the register will automaticalley clear the interrupt flag
	adc.bindex=adc.bindex+32;
		if(adc.bindex==256)
		{
			adc.bindex=0;
			adc.bufferfull=1;
			STOP_ADC_SAMPLING;
			P3OUT &=~BIT4;							//debugging stuff
		}
}


#define CMSTATE_idle							0x00
#define CMSTATE_start_edge_occured_capturing 	0x01
#define CMSTATE_wait_read 						0x02
#define CMSTATE_over_flow_occured 				0x03
#define CMSTATE_end_edge_occured 				0x04

#pragma vector = TIMER0_B1_VECTOR //ccr
__interrupt void TIMERA0_OV_ISR(void)
{
	P2OUT ^= BIT6;									//debugging stuff
	if(TB0CCTL1 &CCIFG){
	if(cap.cap_m_state==CMSTATE_idle)
		{
		if(adc.trigger)
		{
			P3OUT |=BIT4;							//debugging stuff
			TA1R=0;
			START_ADC_SAMPLING;
			adc.trigger=0;
		}

			if(TB0CCTL1 & CCI){
				TB0CTL &= ~TBIFG;
				TB0R=0x07; //as around 3.88us got so /500ns totoal 7 cycle
				TB0CTL |= TBIE ; // enable interrupt and clear the timer
				cap.cap_m_state=CMSTATE_end_edge_occured;
			}
			cap.flush=TB0CCR1;
		}else if(cap.cap_m_state==CMSTATE_end_edge_occured  )
		{
			cap.data+=TB0CCR1;
			cap.flag++;
			if(cap.flag==4)
			{
				TB0CCTL1 &= ~(CCIFG |COV);
				TB0CCTL1 &=~CCIE;
			}
			TB0CTL &= ~TBIFG;
			TB0CTL &= ~TBIE;
			cap.cap_m_state=CMSTATE_idle;
		}else if(cap.cap_m_state==CMSTATE_over_flow_occured)
		{
			cap.flush=TB0CCR1;
			cap.flag =0;
			cap.cap_m_state=CMSTATE_idle;
			TB0CTL &= ~TBIE;
			TB0CTL &= ~TBIFG;
		}
	TB0CCTL1 &= ~(CCIFG |COV);
	}else if (TB0CTL & TBIFG)
	{

		cap.flag =0;
		cap.cap_m_state= CMSTATE_over_flow_occured;
		TB0CTL &= ~TBIFG;
		TB0CTL &= ~TBIE; // disable the interrupt
	}


}

int main(void) {

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
   // CSCTL0 =0xA200;
    P1DIR = BIT6|BIT2; //mosi
	P2DIR =	BIT2|BIT5|BIT6; //clk
	P4DIR = BIT6;
	P3DIR = BIT5|BIT4|BIT6;
	P3SEL1 = BIT5;	// compra
	PJDIR = BIT1; //mclk out
	PJSEL0 = BIT1|BIT6|BIT7;	// mclk out
	P1SEL1 = BIT6|BIT7|BIT4 ;// mosi and miso
	P1SEL0 = BIT4|BIT2;// bit2 ccpout
	P2SEL1 = BIT2|BIT4;	//clock
	P2SEL0 = BIT4; // 2.4 is analog in

   	PM5CTL0=0;
    CSCTL0 =0xA500;
     CSCTL1 = DCORSEL | DCOFSEL2;
     CSCTL3 &= ~(DIVS0 | DIVS1 | DIVS2 |  DIVM1 | DIVM2 );
  //   CSCTL2 |= SELA0;													//clock need to be fixed
  //   CSCTL4 = HFXTDRIVE0|HFXTDRIVE1 | HFFREQ1 |LFXTOFF  ;
  //   CSCTL5 &= ~(HFXTOFFG | LFXTOFFG);
  //   SFRIFG1 &= ~(OFIFG);
   //  __delay_cycles(400000);
   //  CSCTL2 = SELS0 | SELS2 | SELM0 |SELM2 | SELA0;

       LCD_init();
       initCOMP();
       initADC();

   //    LCD_clear();


       TIMER_B_initCapture();
       cap.flag =0;
       cap.cap_m_state = CMSTATE_idle;
       TIMER_A1_initSampler();
       TIMER_A0_initScreenUpdater();
      __enable_interrupt();

    while(1)
    {
    }

}
