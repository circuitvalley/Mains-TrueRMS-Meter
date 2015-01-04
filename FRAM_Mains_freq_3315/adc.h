/*
 * adc.h
 *
 *  Created on: Dec 26, 2014
 *      Author: Gaurav
 */

#ifndef ADC_H_
#define ADC_H_

void initADC();
#define STOP_ADC_SAMPLING 	ADC12CTL0 &= ~ADC12ENC

#define START_ADC_SAMPLING	ADC12CTL0 |= ADC12ENC

#endif /* ADC_H_ */
