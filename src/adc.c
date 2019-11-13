/*
 * adc.c
 *	Interface to EFR32 Blue Gecko ADC HAL
 *  Created on: Nov 11, 2019
 *      Author: vkronber
 */

#include "adc.h"

#define adcFreq (1600000)

void adc_init()
{

	// Enable ADC0 clock
	CMU_ClockEnable(cmuClock_ADC0, true);

	// Declare init structs
	ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
	ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

	// Modify init structs and initialize
	init.prescale = ADC_PrescaleCalc(adcFreq, 0); // Init to max ADC clock for Series 1

	initSingle.diff       = false;        // single ended
	initSingle.reference  = adcRef2V5;    // internal 2.5V reference
	initSingle.resolution = adcRes12Bit;  // 12-bit resolution
	initSingle.acqTime    = adcAcqTime4;  // set acquisition time to meet minimum requirement

	// Select ADC input. See README for corresponding EXP header pin.
	initSingle.posSel = adcPosSelAPORT2XCH9;
	init.timebase = ADC_TimebaseCalc(0);

	ADC_Init(ADC0, &init);
	ADC_InitSingle(ADC0, &initSingle);

}
