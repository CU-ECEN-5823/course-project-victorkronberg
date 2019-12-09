/*
 * persistent_store.h
 *
 *  Created on: Dec 7, 2019
 *      Author: vkronber
 */

#ifndef SRC_PERSISTENT_STORE_H_
#define SRC_PERSISTENT_STORE_H_

#include "main.h"

// Number of storage keys MUST be power of 2 for proper utilization of ring buffer
#define NUM_STORAGE_KEYS	(64)
#define PS_BASE_KEY			(0x4001)

struct ps_pointers
{
	uint8_t ps_head;
	uint8_t ps_tail;
} ps_pointers;

struct sensor_struct
{
	uint32_t timestamp;
	present_input_voltage_t soil_moisture;
	illuminance_t illuminance;
	temperature_8_t temperature;

};

void persistent_storage_init(void);

void persistent_storage_print_all(void);

void persistent_storage_save(struct sensor_struct sensors);

uint32_t ps_buffer_length(void);


#endif /* SRC_PERSISTENT_STORE_H_ */
