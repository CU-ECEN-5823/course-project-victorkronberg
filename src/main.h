/*
 * main.h
 *
 *  Created on: Nov 11, 2019
 *      Author: vkronber
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

#include <stdbool.h>
#include "native_gecko.h"
#include "log.h"
#include "gpio.h"
#include "gecko_ble_errors.h"
#include "gecko_main.h"
#include "ble_mesh_device_type.h"
#include "mesh_generic_model_capi_types.h"
#include "timer.h"
#include "adc.h"
#include "light_sensor.h"
#include "gecko_mesh.h"
#include "server.h"
#include "client.h"
#include "lcd_display.h"
#include "scheduler.h"

volatile uint32_t sample;
volatile uint32_t millivolts;

#endif /* SRC_MAIN_H_ */
