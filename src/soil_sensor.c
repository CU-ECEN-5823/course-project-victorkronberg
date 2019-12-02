/*
 * soil_sensor.c
 *
 *  Created on: Nov 29, 2019
 *      Author: vkronber
 */


#include "soil_sensor.h"

uint32_t soil_sensor_get_voltage(void)
{
	return sensor_data.data.soil_moisture;
}
