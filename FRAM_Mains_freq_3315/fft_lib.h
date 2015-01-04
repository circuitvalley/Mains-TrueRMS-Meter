/*
 * fft_lib.h
 *
 *  Created on: Jan 2, 2015
 *      Author: Gaurav
 */

#ifndef FFT_LIB_H_
#define FFT_LIB_H_

#define SAMPLE_COUNT 256

uint16_t sqrt430(uint32_t ninp);
uint16_t calcTRMS (uint16_t*  );	//return x100 volts for floating adjustments

#endif /* FFT_LIB_H_ */
