/*
 * adc.h
 *
 *  Created on: Nov 11, 2019
 *      Author: vkronber
 */

#ifndef SRC_ADC_H_
#define SRC_ADC_H_

#include "main.h"
#include "em_adc.h"

#define ADC_MEASURE_LIGHT	(1)
#define ADC_MEASURE_SOIL	(0)

#define ADC0_EXPANSION_PIN_12	adcPosSelAPORT4YCH8
#define ADC0_EXPANSION_PIN_14	adcPosSelAPORT4XCH9

void adc_init(void);

#endif /* SRC_ADC_H_ */
