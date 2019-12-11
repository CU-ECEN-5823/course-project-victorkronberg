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

	// Declare init structs
	ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
	ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

	// Enable ADC clock
	//CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_ADC0, true);

	// Select AUXHFRCO for ADC ASYNC mode so it can run in EM2
	ADC0->CTRL = ADC_CTRL_ADCCLKMODE_ASYNC;
	// Make this change to ensure the AUXHFRCO is selected
	CMU->ADCCTRL = CMU_ADCCTRL_ADC0CLKSEL_AUXHFRCO;

	// Set AUXHFRCO frequency and use it to setup the ADC
	CMU_AUXHFRCOFreqSet(cmuAUXHFRCOFreq_1M0Hz);
	init.timebase = ADC_TimebaseCalc(CMU_AUXHFRCOBandGet());
	init.prescale = ADC_PrescaleCalc(adcFreq, CMU_AUXHFRCOBandGet());

	// Let the ADC enable its clock in EM2 when necessary
	init.em2ClockConfig = adcEm2ClockOnDemand;

	initSingle.diff       = false;        // single ended
	initSingle.reference  = adcRef5V;    // internal 2.5V reference
	initSingle.resolution = adcRes12Bit;  // 12-bit resolution
	initSingle.acqTime    = adcAcqTime4;  // set acquisition time to meet minimum requirement

	// Select ADC input. See README for corresponding EXP header pin.
	// PC9 - ADC0 Port 2X Channel 9 (Expansion Header Pin 10) - Light Sensor
	initSingle.posSel = ADC0_EXPANSION_PIN_10;

	ADC_Init(ADC0, &init);
	ADC_InitSingle(ADC0, &initSingle);

	// Enable ADC Single Conversion Complete interrupt
	ADC_IntEnable(ADC0, ADC_IEN_SINGLE);

	// Enable ADC interrupts
	NVIC_ClearPendingIRQ(ADC0_IRQn);
	NVIC_EnableIRQ(ADC0_IRQn);
}

void adc_set_light_sensor(void)
{
	ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

	initSingle.diff       = false;        // single ended
	initSingle.reference  = adcRef5V;    // internal 2.5V reference
	initSingle.resolution = adcRes12Bit;  // 12-bit resolution
	initSingle.acqTime    = adcAcqTime4;  // set acquisition time to meet minimum requirement

	// Select ADC input. See README for corresponding EXP header pin.
	// PC9 - ADC0 Port 2X Channel 9 (Expansion Header Pin 10) - Light Sensor
	initSingle.posSel = ADC0_EXPANSION_PIN_10;

	ADC_InitSingle(ADC0, &initSingle);

}

void adc_set_soil_sensor(void)
{
	ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

	initSingle.diff       = false;        // single ended
	initSingle.reference  = adcRef5V;    // internal 2.5V reference
	initSingle.resolution = adcRes12Bit;  // 12-bit resolution
	initSingle.acqTime    = adcAcqTime4;  // set acquisition time to meet minimum requirement

	// PC9 - ADC0 Port 4X Channel 3 (Expansion Header Pin 9) - Soil Moisture
	initSingle.posSel = ADC0_EXPANSION_PIN_9;

	ADC_InitSingle(ADC0, &initSingle);
}

/**************************************************************************//**
 * @brief  ADC Handler
 *****************************************************************************/
void ADC0_IRQHandler(void)
{

	__disable_irq();
	// Get ADC result
	sample = ADC_DataSingleGet(ADC0);

	gecko_external_signal(ADC_EVENT_MASK);

	__enable_irq();


}
