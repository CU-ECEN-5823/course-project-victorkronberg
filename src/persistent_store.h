/*
 * persistent_store.h
 * Header for persistent storage functions
 *
 *  Created on: Dec 7, 2019
 *      Author: vkronber
 */

#ifndef SRC_PERSISTENT_STORE_H_
#define SRC_PERSISTENT_STORE_H_

#include "main.h"

// Number of storage keys MUST be power of 2 for proper utilization of ring buffer
#define NUM_STORAGE_KEYS	(16)
#define PS_BASE_KEY			(0x4001)

// Persistent storage buffer structure
struct ps_pointers
{
	uint8_t ps_head;
	uint8_t ps_tail;
} ps_pointers;

//Structure for storing sensor data in flash
struct sensor_struct
{
	uint32_t timestamp;
	present_input_voltage_t soil_moisture;
	illuminance_t illuminance;
	temperature_8_t temperature;

};

/**
 * [persistent_storage_init]
 * @description: 	Initializes persistent storage circular buffer pointers
 */
void persistent_storage_init(void);

/**
 * [persistent_storage_print_all]
 * @description: 	Prints all sensor data stored in persistent memory.  Also saves
 * 								pointers to persistent memory.  Pointers are not stored at every
 * 								data point to prevent excessive writing to flash.
 */
void persistent_storage_print_all(void);

/**
 * [persistent_storage_save]
 * @description:	Saves the passed sensor struct to the "head" of the circular buffer
 * 								and the head pointer is incremenetedIf the circular buffer is full,
 * 								the tail is incremented and the data point is stored at that location.
 * @param        sensors [struct containing sensor data and timestamp]
 */
void persistent_storage_save(struct sensor_struct sensors);

void persistent_storage_restore(void);

/**
 * [ps_buffer_length]
 * @description:	Calulates the number of data pointers stored in persistent memory
 * 								based on the head/tail pointers.  Returns this number as a uint32_t.
 * @return        [Number of data points stored in persistent memory]
 */
uint32_t ps_buffer_length(void);


#endif /* SRC_PERSISTENT_STORE_H_ */
