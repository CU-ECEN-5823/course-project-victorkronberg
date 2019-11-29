/*
 * analog_sensors.c
 *
 *  Created on: Nov 11, 2019
 *      Author: vkronber
 */

#include "light_sensor.h"

uint32_t light_sensor_get_illuminance(void)
{
	return sensor_data.data.lightness;
}
