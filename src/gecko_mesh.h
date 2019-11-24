/*
 * gecko_mesh.h
 * Interface with Silicon Labs mesh API
 *
 *  Created on: Nov 4, 2019
 *      Author: vkronber
 */

#ifndef SRC_GECKO_MESH_H_
#define SRC_GECKO_MESH_H_

#include "main.h"
#include "mesh_generic_model_capi_types.h"
#include <mesh_lib.h>

/*******************************************************************************
 * Timer handles defines.
 ******************************************************************************/
#define TIMER_ID_RESTART          	78
#define TIMER_ID_FACTORY_RESET    	77
#define TIMER_ID_PROVISIONING     	66
#define TIMER_ID_RETRANS          	10
#define TIMER_ID_FRIEND_FIND      	20
#define TIMER_ID_NODE_CONFIGURED	30
#define TIMER_ID_DELAYED_ONOFF		51
#define TIMER_ID_ONOFF_TRANSITION	53
#define TIMER_ID_SAVE_STATE			60

// Name length is 12 bytes
#define NAME_LENGTH	(12)

/// Timer Frequency used
#define TIMER_CLK_FREQ ((uint32)32768)
/// Convert miliseconds to timer ticks
#define TIMER_MS_2_TIMERTICK(ms) ((TIMER_CLK_FREQ * ms) / 1000)

#define CONNECTED					1
#define UNCONNECTED					0
#define LPN_INACTIVE				0
#define LPN_ACTIVE					1

// Connection handle
uint8_t conn_handle;
// Connected state
uint32_t connection_state;
// Transaction ID
uint8_t trans_id;
// Number of retransmissions for reliability
uint8_t request_count;
// Element index
uint16_t elem_index;
// My address
uint16_t my_address;
// LPN status
uint8_t lpn_active;

typedef union
{
	volatile uint8_t buffer[8];
	struct data
	{
		volatile uint32_t soil_moisture;
		volatile uint32_t lightness;
	} data;
} SensorData_t;

SensorData_t sensor_data;

/**
 * [gecko_ble_init_LCD_status]
 * @description: Initializes LCD with BT address and server/client status of device
 */
void gecko_mesh_init_LCD_status(struct gecko_msg_system_get_bt_address_rsp_t *bt_address);

/***************************************************************************//**
 * This function is called to initiate factory reset. Factory reset may be
 * initiated by keeping one of the WSTK pushbuttons pressed during reboot.
 * Factory reset is also performed if it is requested by the provisioner
 * (event gecko_evt_mesh_node_reset_id).
 ******************************************************************************/
void gecko_mesh_initiate_factory_reset(void);

/**
 * [gecko_mesh_set_device_name]
 * @description:  Initializes the device name in the BL Mesh stack. Prints
 *                the device name BL address to the LCD
 */
void gecko_mesh_set_device_name(void);

/***************************************************************************//**
 * This function publishes one generic on/off request to change the state
 * of light(s) in the group.
 * param[in] retrans  Indicates if this is the first request or a retransmission,
 *                    possible values are 0 = first request, 1 = retransmission.
 *
 * @note This application sends multiple generic on/off requests for each
 *       button press to improve reliability.
 *       The transaction ID is not incremented in case of a retransmission.
 ******************************************************************************/
void gecko_mesh_send_onoff_request(int retrans);

/**
 * [gecko_mesh_init_models]
 * @description:  Initializes models for Publisher/Subscriber and Server/Client
 *                This function is called after a device that is provisioned has
 *                been initialized or after a a provisioning has occurred.
 */
void gecko_mesh_init_models(void);

void gecko_mesh_lpn_init(void);
void gecko_mesh_lpn_deinit(void);

#endif /* SRC_GECKO_MESH_H_ */
