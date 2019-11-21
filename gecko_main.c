/***************************************************************************//**
 * @file
 * @brief Silicon Labs BT Mesh Empty Example Project
 * This example demonstrates the bare minimum needed for a Blue Gecko BT Mesh C application.
 * The application starts unprovisioned Beaconing after boot
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include <gecko_configuration.h>
#include <mesh_sizes.h>

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
#include <em_gpio.h>

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif
#include "src/main.h"

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

// bluetooth stack heap
#define MAX_CONNECTIONS 2

uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS) + BTMESH_HEAP_SIZE + 1760];

// Bluetooth advertisement set configuration
//
// At minimum the following is required:
// * One advertisement set for Bluetooth LE stack (handle number 0)
// * One advertisement set for Mesh data (handle number 1)
// * One advertisement set for Mesh unprovisioned beacons (handle number 2)
// * One advertisement set for Mesh unprovisioned URI (handle number 3)
// * N advertisement sets for Mesh GATT service advertisements
// (one for each network key, handle numbers 4 .. N+3)
//
#define MAX_ADVERTISERS (4 + MESH_CFG_MAX_NETKEYS)

static gecko_bluetooth_ll_priorities linklayer_priorities = GECKO_BLUETOOTH_PRIORITIES_DEFAULT;

// bluetooth stack configuration
extern const struct bg_gattdb_def bg_gattdb_data;

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;

const gecko_configuration_t config =
{
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.max_advertisers = MAX_ADVERTISERS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap) - BTMESH_HEAP_SIZE,
  .bluetooth.sleep_clock_accuracy = 100,
  .bluetooth.linklayer_priorities = &linklayer_priorities,
  .gattdb = &bg_gattdb_data,
  .btmesh_heap_size = BTMESH_HEAP_SIZE,
#if (HAL_PA_ENABLE)
  .pa.config_enable = 1, // Set this to be a valid PA config
#if defined(FEATURE_PA_INPUT_FROM_VBAT)
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#else
  .pa.input = GECKO_RADIO_PA_INPUT_DCDC,
#endif // defined(FEATURE_PA_INPUT_FROM_VBAT)
#endif // (HAL_PA_ENABLE)
  .max_timers = 16,
};

void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt);
void mesh_native_bgapi_init(void);
bool mesh_bgapi_listener(struct gecko_cmd_packet *evt);

/**
 * See light switch app.c file definition
 */
void gecko_bgapi_classes_init_server_friend(void)
{
	gecko_bgapi_class_dfu_init();
	gecko_bgapi_class_system_init();
	gecko_bgapi_class_le_gap_init();
	gecko_bgapi_class_le_connection_init();
	//gecko_bgapi_class_gatt_init();
	gecko_bgapi_class_gatt_server_init();
	gecko_bgapi_class_hardware_init();
	gecko_bgapi_class_flash_init();
	gecko_bgapi_class_test_init();
	//gecko_bgapi_class_sm_init();
	//mesh_native_bgapi_init();
	gecko_bgapi_class_mesh_node_init();
	//gecko_bgapi_class_mesh_prov_init();
	gecko_bgapi_class_mesh_proxy_init();
	gecko_bgapi_class_mesh_proxy_server_init();
	//gecko_bgapi_class_mesh_proxy_client_init();
	//gecko_bgapi_class_mesh_generic_client_init();
	gecko_bgapi_class_mesh_generic_server_init();
	//gecko_bgapi_class_mesh_vendor_model_init();
	//gecko_bgapi_class_mesh_health_client_init();
	//gecko_bgapi_class_mesh_health_server_init();
	//gecko_bgapi_class_mesh_test_init();
	gecko_bgapi_class_mesh_lpn_init();
	//gecko_bgapi_class_mesh_friend_init();
}


/**
 * See main function list in soc-btmesh-switch project file
 */
void gecko_bgapi_classes_init_client_lpn(void)
{
	gecko_bgapi_class_dfu_init();
	gecko_bgapi_class_system_init();
	gecko_bgapi_class_le_gap_init();
	gecko_bgapi_class_le_connection_init();
	//gecko_bgapi_class_gatt_init();
	gecko_bgapi_class_gatt_server_init();
	gecko_bgapi_class_hardware_init();
	gecko_bgapi_class_flash_init();
	gecko_bgapi_class_test_init();
	//gecko_bgapi_class_sm_init();
	//mesh_native_bgapi_init();
	gecko_bgapi_class_mesh_node_init();
	//gecko_bgapi_class_mesh_prov_init();
	gecko_bgapi_class_mesh_proxy_init();
	gecko_bgapi_class_mesh_proxy_server_init();
	//gecko_bgapi_class_mesh_proxy_client_init();
	gecko_bgapi_class_mesh_generic_client_init();
	//gecko_bgapi_class_mesh_generic_server_init();
	//gecko_bgapi_class_mesh_vendor_model_init();
	//gecko_bgapi_class_mesh_health_client_init();
	//gecko_bgapi_class_mesh_health_server_init();
	//gecko_bgapi_class_mesh_test_init();
	gecko_bgapi_class_mesh_lpn_init();
	//gecko_bgapi_class_mesh_friend_init();

}
void gecko_main_init()
{
	LOG_INFO("GECKO_INIT");
  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();

  // Minimize advertisement latency by allowing the advertiser to always
  // interrupt the scanner.
  linklayer_priorities.scan_max = linklayer_priorities.adv_min + 1;

  gecko_stack_init(&config);

  if( DeviceUsesClientModel() ) {
	  gecko_bgapi_classes_init_client_lpn();
  } else {
	  gecko_bgapi_classes_init_server_friend();
  }

  // Initialize coexistence interface. Parameters are taken from HAL config.
  gecko_initCoexHAL();

}

void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt)
{
  uint16_t result;

  //LOG_INFO("Event %08x",evt_id);

  switch (evt_id) {

  	case gecko_evt_system_boot_id:
		// Initialize unconnected
		connection_state = UNCONNECTED;
		lpn_active = LPN_INACTIVE;
		gpio_get_button_state();
		if(GPIO_PinInGet(PD0_BUTTON_PORT,PD0_BUTTON_PIN)==0 || GPIO_PinInGet(PD1_BUTTON_PORT,PD1_BUTTON_PIN)==0)
		{
			LOG_INFO("Factory Reset");
			gecko_mesh_initiate_factory_reset();
		}
		else
		{
			gecko_mesh_set_device_name();
			// Initialize Mesh stack in Node operation mode, wait for initialized event
			BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_node_init());
		}
		break;
	case gecko_evt_mesh_node_initialized_id:
		if (!evt->data.evt_mesh_node_initialized.provisioned)
		{
			//LOG_INFO("Device not provisioned");
			// The Node is now initialized, start unprovisioned Beaconing using PB-ADV and PB-GATT Bearers
			BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_node_start_unprov_beaconing(0x3));
		}
		else
		{
			//LOG_INFO("Device is Provisioned");
			displayPrintf(DISPLAY_ROW_ACTION,"Provisioned");

			// Store address of node
			my_address = evt->data.evt_mesh_node_initialized.address;
			// Initialize models
			gecko_mesh_init_models();
		}
		break;
	case gecko_evt_mesh_node_provisioning_started_id:
		displayPrintf(DISPLAY_ROW_ACTION,"Provisioning");
		break;
	case gecko_evt_mesh_node_provisioned_id:
		// Initialize models
		gecko_mesh_init_models();
		break;
	case gecko_evt_mesh_node_provisioning_failed_id:
		displayPrintf(DISPLAY_ROW_ACTION,"Provision Fail");
		break;
	case gecko_evt_le_connection_opened_id:
		displayPrintf(DISPLAY_ROW_CONNECTION,"Connected");
		connection_state = CONNECTED;
		conn_handle = evt->data.evt_le_connection_opened.connection;
		break;
	case gecko_evt_mesh_generic_server_client_request_id:
		if(!DEVICE_IS_ONOFF_PUBLISHER)
		{
			LOG_INFO("evt gecko_evt_mesh_generic_server_client_request_id\r\n");
			// pass the server client request event to mesh lib handler that will invoke
			// the callback functions registered by application
			mesh_lib_generic_server_event_handler(evt);
		}
		break;

	case gecko_evt_mesh_generic_server_state_changed_id:
		if(!DEVICE_IS_ONOFF_PUBLISHER)
		{
			// uncomment following line to get debug prints for each server state changed event
			//server_state_changed(&(evt->data.evt_mesh_generic_server_state_changed));

			// pass the server state changed event to mesh lib handler that will invoke
			// the callback functions registered by application
			mesh_lib_generic_server_event_handler(evt);
		}
		break;
	case gecko_evt_le_connection_closed_id:
		connection_state = UNCONNECTED;
		displayPrintf(DISPLAY_ROW_CONNECTION,"");
		/* Check if need to boot to dfu mode */
		if (boot_to_dfu)
		{
			/* Enter to DFU OTA mode */
			gecko_cmd_system_reset(2);
		}
		break;
	case gecko_evt_gatt_server_user_write_request_id:
		if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control)
		{
			/* Set flag to enter to OTA mode */
			boot_to_dfu = 1;
			/* Send response to Write Request */
			gecko_cmd_gatt_server_send_user_write_response(
			evt->data.evt_gatt_server_user_write_request.connection,
			gattdb_ota_control,
			bg_err_success);

			/* Close connection to enter to DFU OTA mode */
			gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);
		}
		break;
	case gecko_evt_hardware_soft_timer_id:
		switch (evt->data.evt_hardware_soft_timer.handle)
		{
			case TIMER_ID_FACTORY_RESET:
				// reset the device to finish factory reset
				gecko_cmd_system_reset(0);
				break;
			case TIMER_ID_RETRANS:
				if(DEVICE_IS_ONOFF_PUBLISHER)
				{
					gecko_mesh_send_onoff_request(1);   // param 1 indicates that this is a retransmission
					// stop retransmission timer if it was the last attempt
					if (request_count == 0) {
						gecko_cmd_hardware_set_soft_timer(0, TIMER_ID_RETRANS, 0);
					}
				}
				break;
			case TIMER_ID_SAVE_STATE:
				if(!DEVICE_IS_ONOFF_PUBLISHER)
				{
					server_on_off_state_store();
				}
				break;
	        case TIMER_ID_FRIEND_FIND:
	          printf("trying to find friend...\r\n");
	          result = gecko_cmd_mesh_lpn_establish_friendship(0)->result;
	        break;
		}
		break;
	case gecko_evt_mesh_node_reset_id:
		break;
	case gecko_evt_mesh_lpn_friendship_established_id:
	  printf("friendship established\r\n");
	  //DI_Print("LPN with friend", DI_ROW_LPN);
	  break;

	case gecko_evt_mesh_lpn_friendship_failed_id:
	  printf("friendship failed\r\n");
	  //DI_Print("no friend", DI_ROW_LPN);
	  // try again in 2 seconds
	  result = gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(2000),
												 TIMER_ID_FRIEND_FIND,
												 1)->result;
	  if (result) {
		printf("timer failure?!  %x\r\n", result);
	  }
	  break;

	case gecko_evt_mesh_lpn_friendship_terminated_id:
	  printf("friendship terminated\r\n");
	  //DI_Print("friend lost", DI_ROW_LPN);
	  //if (num_connections == 0) {
		// try again in 2 seconds
		result = gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(2000),
												   TIMER_ID_FRIEND_FIND,
												   1)->result;
		if (result) {
		  printf("timer failure?!  %x\r\n", result);
		}
	  //}
	  break;
	case gecko_evt_system_external_signal_id:
		switch (evt->data.evt_system_external_signal.extsignals)
		{
			case BUTTON_EVENT_MASK:
				__disable_irq();
				my_state_struct.event_bitmask |= BUTTON_EVENT_MASK;
				__enable_irq();
				break;
			case ONE_HZ_EVENT_MASK:
				__disable_irq();
				my_state_struct.event_bitmask |= ONE_HZ_EVENT_MASK;
				__enable_irq();
				break;
			case SENSE_EVENT_MASK:
				__disable_irq();
				my_state_struct.event_bitmask |= SENSE_EVENT_MASK;
				__enable_irq();
				break;
			case ADC_EVENT_MASK:
				__disable_irq();
				my_state_struct.event_bitmask |= ADC_EVENT_MASK;
				__enable_irq();
				break;
		}
		break;
	default:
		break;
  }
}
