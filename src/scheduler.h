/*
 * scheduler.h
 *
 * Header file for scheduler
 *
 *  Created on: Sep 21, 2019
 *      Author: Victor Kronberg
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include "main.h"
#include "sleep.h"

#define ZERO_DELAY	(0)

#define SCHEDULER_SUPPORTS_DISPLAY_UPDATE_EVENT 	(1)
#define TIMER_SUPPORTS_1HZ_TIMER_EVENT				(1)

// Event bitmasks
#define TIMER_EVENT_MASK			(0x0001)
#define TIMER_EVENT_MASK_POS		(0)
#define SENSE_EVENT_MASK			(0x0002)
#define SENSE_EVENT_MASK_POS		(1)
#define I2C_EVENT_MASK				(0x0004)
#define I2C_EVENT_MASK_POS			(2)
#define BLE_EVENT_MASK				(0x0008)
#define BLE_EVENT_MASK_POS			(3)
#define EXIT_EVENT_MASK				(0x0010)
#define EXIT_EVENT_MASK_POS			(4)
#define ONE_HZ_EVENT_MASK			(0x0020)
#define ONE_HZ_EVENT_MASK_POS		(5)
#define PASSKEY_CONFIRM_MASK		(0x0040)
#define PASSKEY_CONFIRM_POS			(6)
#define BUTTON_EVENT_MASK			(0x0080)
#define BUTTON_EVENT_POS			(7)
#define ADC_EVENT_MASK				(0x0100)
#define ADC_EVENT_POS				(8)
#define DELAY_EVENT_MASK			(0x0200)
#define DELAY_EVENT_MASK_POS		(9)

typedef enum states  {
	STATE0_WAIT_FOR_TIMER,
	STATE1_MEASURE_LIGHT,
	STATE2_MEASURE_SOIL,
	STATE3_I2C_WRITE,
	STATE4_I2C_WAIT,
	STATE5_I2C_READ,
	MY_NUM_STATES
} myState;

typedef struct	StateStruct {
	myState current_state;
	myState next_state;
	uint32_t event_bitmask;
	uint32_t periodic_timer0;
} myStateTypeDef;


// Global state structure
myStateTypeDef my_state_struct;

// Global temperature value
uint32_t temp_value;

/**
 * [my_scheduler]
 * @description: Interrupt driven scheduler which controls server state machine. Handles
 * 							changes between states and calling functions relevant to state
 * 							operations.
 * @param        state_struct [Struct containing context for the state machine]
 */
void my_scheduler(myStateTypeDef *state_struct);

/**
 * [init_scheduler]
 * @description:	Initializes scheduler for 1Hz repeating timer
 */
void init_scheduler(void);

/**
 * [scheduler_one_hz_event_handler]
 * @description: Event handler for 1Hz events. Updates TX power based on current
 * 							RSSI value and toggles EXTCOMIN on LCD to prevent charge buildup
 */
void scheduler_one_hz_event_handler(void);

void scheduler_sensor_event_handler(void);

/**
 * [scheduler_confirm_passkey]
 * @description:	Scheduler event to handle user confirmation of BLE pairing
 * 								passkey.  Clears bitmask and calls BLE function to confirm
 * 								passkey.
 * @param        state_struct [Struct containing context for the state machine]
 */
void scheduler_confirm_passkey(myStateTypeDef *state_struct);

/**
 * [scheduler_update_button_status]
 * @description:	Scheduler event to handle pressing or releasing of PB0.  Upon
 * 								change to button's state, status is sent as an indication to
 * 								bonded devices.
 * @param        state_struct [Struct containing context for the state machine]
 */
void scheduler_update_button_status(myStateTypeDef *state_struct);

/**
 * [scheduler_start_i2c_write]
 * @description: Set deepest sleep state to EM1 and begin I2C write. To be called
 * 				 			in STATE1_I2C_POWER_UP.
 */
void scheduler_start_i2c_write(void);

/**
 * [scheduler_wait_for_temp_conversion]
 * @description: Set deepest sleep state to EM3 and set delay timer to wait for conversion.
 * 							To be called in STATE2_I2C_WRITE.
 */
void scheduler_wait_for_temp_conversion(void);

/**
 * [scheduler_start_i2c_read]
 * @description: Set deepest sleep to EM1 and start I2C read. To be called in
 * 							STATE3_I2C_WAIT
 */
void scheduler_start_i2c_read(void);

/**
 * [scheduler_return_temp_then_wait]
 * @description: Retrieve temperature from I2C buffer, set deepest sleep to EM3,
 * 							and power down Si7021.  To be called in STATE4_I2C_READ.
 */
void scheduler_return_temp_then_wait(void);

/**
 * [client_scheduler description]
 * @description: Interrupt driven scheduler which controls client state machine. Handles
 * 							changes between states and calling functions relevant to state
 * 							operations.
 * @param        state_struct [Struct containing context for the state machine]
 */
void client_scheduler(myStateTypeDef *state_struct);

#endif /* SRC_SCHEDULER_H_ */
