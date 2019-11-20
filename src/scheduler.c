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

		LOG_INFO("1 HZ Event Handleer");
		scheduler_one_hz_event_handler();

	}

	if( ((state_struct->event_bitmask & BUTTON_EVENT_MASK) >> BUTTON_EVENT_POS) == 1 )
	{
		__disable_irq();
		// Clear event bitmask
		state_struct->event_bitmask &= ~BUTTON_EVENT_MASK;

		request_count = 3;

		__enable_irq();

		LOG_INFO("Sending button state");
		gecko_mesh_send_onoff_request(0);
	}

	switch(state_struct->current_state)
	case STATE1_MEASURE_LIGHT:
		if( ((state_struct->event_bitmask & ADC_EVENT_MASK) >> ADC_EVENT_POS) == 1 )
		{
			__disable_irq();
			// Clear event bitmask
			state_struct->event_bitmask &= ~ADC_EVENT_MASK;

			request_count = 3;

			__enable_irq();

			LOG_INFO("Measuring light");

		}
		break;
	case STATE2_MEASURE_SOIL:
		if( ((state_struct->event_bitmask & ADC_EVENT_MASK) >> ADC_EVENT_POS) == 1 )
		{
			__disable_irq();
			// Clear event bitmask
			state_struct->event_bitmask &= ~ADC_EVENT_MASK;

			request_count = 3;

			__enable_irq();

			LOG_INFO("Measuring soil");

		}
		break;

	if(state_struct->current_state != state_struct->next_state)
	{
		//LOG_INFO("State transitioned from state %d to %d",state_struct->current_state,state_struct->next_state);
		state_struct->current_state = state_struct->next_state;
	}


}

void init_scheduler(void)
{
	__disable_irq();
	my_state_struct.current_state = STATE0_WAIT_FOR_BLE;
	my_state_struct.next_state = STATE0_WAIT_FOR_BLE;
	my_state_struct.event_bitmask = 0;
	__enable_irq();

	// Start the scheduler
	reset_periodic_timer();

}


void scheduler_one_hz_event_handler(void)
{
	// Update LCD display
	displayUpdate();

	// Start ADC conversion
	ADC_Start(ADC0, adcStartSingle);

	// Reset period interrupt
	reset_periodic_timer();
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

	// Check for BLE passkey confirmation event
	if( ((state_struct->event_bitmask & PASSKEY_CONFIRM_MASK) >> PASSKEY_CONFIRM_POS) == 1 )
	{
		scheduler_confirm_passkey(state_struct);
	}

}
