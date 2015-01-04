/*
 * fft_lib.c
 *
 *  Created on: Jan 2, 2015
 *      Author: Gaurav
 */

#include <msp430.h>
#include <stdint.h>
#include "timer.h"
#include "comp_e.h"
#include "adc.h"
#include "fft_lib.h"

uint32_t logger[100];
uint16_t calcTRMS (uint16_t* ADCSamples)	//return x10 volts
{
  static uint8_t couter; //debugging stuff
  uint16_t i;
  uint16_t vTRMS;
  uint32_t temp=0;
  uint16_t max = 0, min = 65535, vOFF;

  // get maximum and maximum to calulate offset of the input signal
  for (i = 0; i < SAMPLE_COUNT; i++) {
    max = (max>ADCSamples[i])?max:ADCSamples[i];
    min = (min<ADCSamples[i])?min:ADCSamples[i];
  }

  vOFF = ((max - min)>>1) + min;   //  (max-min/2)+mincalculate offset
  RESLO = 0x00;					   // clear MPY results to start (sample1^2)+(sample2^2)+....+(sampleN^2) final result goes in to RESLO{31:0} RESHI{31:0}
  RESHI = 0x00;
  for (i = 0; i < SAMPLE_COUNT; i++) {
    if (ADCSamples [i] > vOFF) 					//remove offset from the sample before sqare and addition
    	ADCSamples [i] = (ADCSamples [i] - vOFF);
    else
    	ADCSamples [i] = (vOFF - ADCSamples [i]);

    MAC = ADCSamples[i];			// calculate sampleN^2
    OP2 = ADCSamples[i];

  }
  temp = RESHI;
  temp = (temp<<8) | (RESLO>>8);   //  we have goot all the samples squared and added togather now for 256 samples divide by 256 to get mean value
  	  	  	  	  	  	  	  	   // number of samples are specificly chosen 256 as we have very much advantage in calculating FFT

  logger[couter++]=temp;		//debugging stuff
  if(couter==100)
	  couter=0;
  temp= temp*100;
  temp=sqrt430(temp);			  //finally calculate we get RMS value after square root
  vTRMS=(temp*2355)/10000;
 return (vTRMS);
}

uint16_t sqrt430(uint32_t ninp)  //square root
{
	uint16_t i;
	uint32_t temp;
	uint16_t guess=0;
	for(i=0x8000;i>0;i=i>>1)
	{
	guess|=i;					//make a guess
	MPY = guess;				//use MPY module to multiply
	OP2 = guess;
	temp= RESHI;
	temp = temp<<16 | RESLO;
	if(temp>ninp)
	guess &= ~(i);


	}
	return guess;
}
