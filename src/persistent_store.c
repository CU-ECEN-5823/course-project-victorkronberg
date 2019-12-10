/*
 * persistent_store.c
 *
 *  Created on: Dec 7, 2019
 *      Author: vkronber
 */


#include "persistent_store.h"



void persistent_storage_init(void)
{
	ps_pointers.ps_head = 0;
	ps_pointers.ps_tail = 0;

	gecko_cmd_flash_ps_save(0x4000, sizeof(struct ps_pointers), (const uint8*)&ps_pointers);

}

void persistent_storage_restore(void)
{
	struct gecko_msg_flash_ps_load_rsp_t *response;
	response = gecko_cmd_flash_ps_load(0x4000);
	if (response->result || (response->value.len != sizeof(struct ps_pointers)))
	{
		ps_pointers.ps_head = 0;
		ps_pointers.ps_tail = 0;
	}
	else
	{
		memcpy(&ps_pointers, response->value.data, response->value.len);
	}

	LOG_INFO("Persistent memory set. Head: %d Tail: %d",ps_pointers.ps_head,ps_pointers.ps_tail);
	LOG_INFO("Size of sensor struct: %d bytes", sizeof(struct sensor_struct));
	displayPrintf(DISPLAY_ROW_FLEX_DATA,"Data points: %d",ps_buffer_length());

}

void persistent_storage_print_all(void)
{
	uint32_t index;
	struct sensor_struct sensors;
	struct gecko_msg_flash_ps_load_rsp_t *response;
	for(index = ps_pointers.ps_tail; (ps_pointers.ps_head - index) > 0 ; index++)
	{
		response = gecko_cmd_flash_ps_load(PS_BASE_KEY + (index & (NUM_STORAGE_KEYS - 1)));
		if (response->result || (response->value.len != sizeof(struct sensor_struct)))
		{
			LOG_INFO("Stored element %d does not match sensor struct",index);
		}
		else
		{
			memcpy(&sensors, response->value.data, response->value.len);
			LOG_INFO("Data point: %d of %d",((index - ps_pointers.ps_tail) + 1),ps_buffer_length());
			LOG_INFO("time: %d", sensors.timestamp);
			LOG_INFO("Temperature: %3d.%1d C",sensors.temperature / 2, (sensors.temperature * 5) % 10);
			LOG_INFO("Illuminance: %d.%02d %%",sensors.illuminance / 50,(sensors.illuminance * 2) % 100);
			LOG_INFO("Moisture: %d.%02d %%\n\r",(sensors.soil_moisture / 50),(sensors.soil_moisture * 2) % 100);
		}
	}

	// Save current pointers when you print
	gecko_cmd_flash_ps_save(0x4000, sizeof(struct ps_pointers), (const uint8*)&ps_pointers);
}

void persistent_storage_save(struct sensor_struct sensors)
{

	if(ps_buffer_length() > (NUM_STORAGE_KEYS - 1) )
	{
		ps_pointers.ps_tail++;
	}
	// Save utilizing ring buffer strategy
	LOG_INFO("Data stored at Head: %d and Tail: %d",ps_pointers.ps_head,ps_pointers.ps_tail);
	gecko_cmd_flash_ps_save(PS_BASE_KEY + (ps_pointers.ps_head & (NUM_STORAGE_KEYS - 1)), sizeof(struct sensor_struct), (const uint8*)&sensors);
	ps_pointers.ps_head++;
	displayPrintf(DISPLAY_ROW_FLEX_DATA,"Data points: %d",ps_buffer_length());

}

uint32_t ps_buffer_length(void)
{
	return ps_pointers.ps_head - ps_pointers.ps_tail;
}

