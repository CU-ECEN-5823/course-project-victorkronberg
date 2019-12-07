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
	  CMU->ADCCTRL = CMU_ADCCTRL_ADC0CLKSEL_AUXHFRCO;
	  // Make this change to ensure the AUXHFRCO is selected
	  ADC0->CTRL = ADC_CTRL_ADCCLKMODE_ASYNC;

	  // Set AUXHFRCO frequency and use it to setup the ADC
	  CMU_AUXHFRCOFreqSet(cmuAUXHFRCOFreq_1M0Hz);
	  init.timebase = ADC_TimebaseCalc(CMU_AUXHFRCOBandGet());
	  init.prescale = ADC_PrescaleCalc(adcFreq, CMU_AUXHFRCOBandGet());

	  // Let the ADC enable its clock in EM2 when necessary
	  init.em2ClockConfig = adcEm2ClockOnDemand;
/*
	// Enable ADC0 clock
	CMU_ClockEnable(cmuClock_ADC0, true);
*/
	// Declare init structs
	//ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
	//ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

	// Modify init structs and initialize
	//init.prescale = ADC_PrescaleCalc(adcFreq, 0); // Init to max ADC clock for Series 1

	initSingle.diff       = false;        // single ended
	initSingle.reference  = adcRef5V;    // internal 2.5V reference
	initSingle.resolution = adcRes12Bit;  // 12-bit resolution
	initSingle.acqTime    = adcAcqTime4;  // set acquisition time to meet minimum requirement

	// Select ADC input. See README for corresponding EXP header pin.
	// PC9 - ADC0 Port 2X Channel 9 (Expansion Header Pin 10)
	//initSingle.posSel = adcPosSelAPORT2XCH9;
	// PC9 - ADC0 Port 4X Channel 3 (Expansion Header Pin 9)
	initSingle.posSel = adcPosSelAPORT4XCH3;
	//init.timebase = ADC_TimebaseCalc(0);

	ADC_Init(ADC0, &init);
	ADC_InitSingle(ADC0, &initSingle);

	// Enable ADC Single Conversion Complete interrupt
	ADC_IntEnable(ADC0, ADC_IEN_SINGLE);

	// Enable ADC interrupts
	NVIC_ClearPendingIRQ(ADC0_IRQn);
	NVIC_EnableIRQ(ADC0_IRQn);
}

/**************************************************************************//**
 * @brief  ADC Handler
 *****************************************************************************/
void ADC0_IRQHandler(void)
{
	//
	//
	//
	//
	//while(!(ADC0->STATUS & _ADC_STATUS_SINGLEDV_MASK));
	// Do we need to check to ensure Conversion is complete?
	//
	//
	//
	//

	__disable_irq();
	// Get ADC result
	sample = ADC_DataSingleGet(ADC0);

	//LOG_INFO("Raw data: %d",sample);



	//if(my_state_struct.current_state == STATE1_MEASURE_LIGHT)
	//{
		// Set Gecko event to read next
		gecko_external_signal(ADC_EVENT_MASK);
	//}

	__enable_irq();


}
