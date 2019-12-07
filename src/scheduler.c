/*
 * scheduler.c
 *
 * State-machine for handling event-driven processes external to BLE
 *
 *  Created on: Sep 21, 2019
 *      Author: Victor Kronberg
 */


#include "scheduler.h"

void my_scheduler(myStateTypeDef *state_struct)
{

	// Check for ONE HZ EVENT
	if( ((state_struct->event_bitmask & ONE_HZ_EVENT_MASK) >> ONE_HZ_EVENT_MASK_POS) == 1 )
	{
		__disable_irq();
		// Clear event bitmask
		state_struct->event_bitmask &= ~ONE_HZ_EVENT_MASK;
		__enable_irq();

		//LOG_INFO("1 HZ Event Handler");
		scheduler_one_hz_event_handler();

	}

	if( ((state_struct->event_bitmask & BUTTON_EVENT_MASK) >> BUTTON_EVENT_POS) == 1 )
	{
		__disable_irq();
		// Clear event bitmask
		state_struct->event_bitmask &= ~BUTTON_EVENT_MASK;

		request_count = 3;

		__enable_irq();

		//LOG_INFO("Sending button state");
		gecko_mesh_send_onoff_request(0);
	}

	switch(state_struct->current_state)
	{
		case STATE0_WAIT_FOR_TIMER:
			if( ((state_struct->event_bitmask & SENSE_EVENT_MASK) >> SENSE_EVENT_MASK_POS) == 1 )
			{
				__disable_irq();
				// Clear event bitmask
				state_struct->event_bitmask &= ~SENSE_EVENT_MASK;
				state_struct->next_state = STATE1_MEASURE_LIGHT;
				__enable_irq();

				ADC_Start(ADC0, adcStartSingle);

				//LOG_INFO("Sense event occurred");
				scheduler_sensor_event_handler();
			}
			break;
		case STATE1_MEASURE_LIGHT:
			if( ((state_struct->event_bitmask & ADC_EVENT_MASK) >> ADC_EVENT_POS) == 1 )
			{
				__disable_irq();
				// Clear event bitmask
				state_struct->event_bitmask &= ~ADC_EVENT_MASK;
				state_struct->next_state = STATE2_MEASURE_SOIL;
				__enable_irq();

				// Calculate input voltage in mV
				millivolts = (sample * 5000) / 4096;

				sensor_data.data.lightness = millivolts;

				ADC_Start(ADC0, adcStartSingle);

				//LOG_INFO("Light Reading: %d",millivolts);


			}
			break;
		case STATE2_MEASURE_SOIL:
			if( ((state_struct->event_bitmask & ADC_EVENT_MASK) >> ADC_EVENT_POS) == 1 )
			{
				__disable_irq();
				// Clear event bitmask
				state_struct->event_bitmask &= ~ADC_EVENT_MASK;
				//state_struct->next_state = STATE0_WAIT_FOR_TIMER;

				__enable_irq();

				// Calculate input voltage in mV
				millivolts = (sample * 5000) / 4096;

				sensor_data.data.soil_moisture = millivolts;

				// Set deepest sleep state to EM1 and begin i2c write
				scheduler_start_i2c_write();

				// Set next state
				state_struct->next_state = STATE3_I2C_WRITE;

				//LOG_INFO("Soil Reading: %d",millivolts);

				//sensor_server_publish();
			}
			break;
		case STATE3_I2C_WRITE:
			if( ((state_struct->event_bitmask & I2C_EVENT_MASK) >> I2C_EVENT_MASK_POS) == 1 )
			{
				__disable_irq();
				// Clear event bitmask
				state_struct->event_bitmask &= ~I2C_EVENT_MASK;
				__enable_irq();

				// Set deepest sleep state to EM3 and set delay timer to wait for conversion
				scheduler_wait_for_temp_conversion();

				// Set next state if complete
				state_struct->next_state = STATE4_I2C_WAIT;
			}
			break;
		case STATE4_I2C_WAIT:
			if( ((state_struct->event_bitmask & DELAY_EVENT_MASK) >> DELAY_EVENT_MASK_POS) == 1 )
			{
				__disable_irq();
				// Clear event bitmask
				state_struct->event_bitmask &= ~DELAY_EVENT_MASK;
				__enable_irq();

				// Set deepest sleep to EM1 and start i2c read
				scheduler_start_i2c_read();

				// Set next state if complete
				state_struct->next_state = STATE5_I2C_READ;
			}
			break;
		case STATE5_I2C_READ:
			// If event flag set for read transfer complete, read temperature from return registers and calc temp
			if( ((state_struct->event_bitmask & I2C_EVENT_MASK) >> I2C_EVENT_MASK_POS) == 1 )
			{
				__disable_irq();
				// Clear event bitmask
				state_struct->event_bitmask &= ~I2C_EVENT_MASK;
				__enable_irq();

				// Retrieve temperature from I2C buffer, set deepest sleep to EM3, and power down Si7021
				scheduler_return_temp_then_wait();

				state_struct->next_state = STATE0_WAIT_FOR_TIMER;
			}
			break;
		default:
			break;
	}

	if(state_struct->current_state != state_struct->next_state)
	{
		//LOG_INFO("State transitioned from state %d to %d",state_struct->current_state,state_struct->next_state);
		state_struct->current_state = state_struct->next_state;
	}


}

void init_scheduler(void)
{
	__disable_irq();
	my_state_struct.current_state = STATE0_WAIT_FOR_TIMER;
	my_state_struct.next_state = STATE0_WAIT_FOR_TIMER;
	my_state_struct.event_bitmask = 0;
	__enable_irq();

	// Start the scheduler
	reset_periodic_timer();

}


void scheduler_one_hz_event_handler(void)
{
	// Update LCD display
	displayUpdate();

	// Reset period interrupt
	reset_periodic_timer();
}

void scheduler_sensor_event_handler(void)
{
	scheduler_one_hz_event_handler();

	// Start ADC conversion
	ADC_Start(ADC0, adcStartSingle);
}

void scheduler_confirm_passkey(myStateTypeDef *state_struct)
{
	__disable_irq();
	// Clear event bitmask
	state_struct->event_bitmask &= ~PASSKEY_CONFIRM_MASK;
	__enable_irq();

	//gecko_ble_security_confirm_passkey();

}

void scheduler_update_button_status(myStateTypeDef *state_struct)
{
	__disable_irq();
	// Clear event bitmask
	state_struct->event_bitmask &= ~BUTTON_EVENT_MASK;
	__enable_irq();

	//gecko_ble_send_button_state();
}

void scheduler_start_i2c_write(void)
{
	// Set deepest sleep mode for I2C write
	SLEEP_SleepBlockBegin(sleepEM2);

	// Start I2C write
	si7021_init_i2c_temp_write();
}

void scheduler_wait_for_temp_conversion(void)
{
	// Go to EM3 for conversion delay wait
	SLEEP_SleepBlockEnd(sleepEM2);

	// Set delay to wait form Si7021 conversion
	delay_ms(CONVERSION_DELAY);
}

void scheduler_start_i2c_read(void)
{
	// Conversion delay is complete, init I2C for read
	si7021_init_i2c_temp_read();

	// Set deepest sleep as EM1 for I2C read
	SLEEP_SleepBlockBegin(sleepEM2);
}

void scheduler_return_temp_then_wait(void)
{
	//uint32_t tempF_value;
	// Calculate temperature from last measured value (milli-degrees C)
	//temp_value = si7021_return_last_temp();


	// Go back to deepest sleep to wait for next temperature reading
	SLEEP_SleepBlockEnd(sleepEM2);
}

void client_scheduler(myStateTypeDef *state_struct)
{
	// Check for RSSI check event - does not impact state
	if( ((state_struct->event_bitmask & ONE_HZ_EVENT_MASK) >> ONE_HZ_EVENT_MASK_POS) == 1 )
	{
		__disable_irq();
		// Clear event bitmask
		state_struct->event_bitmask &= ~ONE_HZ_EVENT_MASK;
		__enable_irq();

		scheduler_one_hz_event_handler();

	}

}
