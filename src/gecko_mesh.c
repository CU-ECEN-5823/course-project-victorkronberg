/*
 * gecko_mesh.c
 * These functions are for interfacing with the Silicon Labs BL Mesh API for
 * both client and server devices.
 *
 *  Created on: Nov 4, 2019
 *      Author: vkronber
 */

#include <src/gecko_mesh.h>

void gecko_mesh_init_LCD_status(struct gecko_msg_system_get_bt_address_rsp_t *bt_address)
{

	if(DeviceIsOnOffPublisher())
	{
	  displayPrintf(DISPLAY_ROW_NAME,"Publisher");
	}
	else
	{
	  displayPrintf(DISPLAY_ROW_NAME,"Subscriber");
	}

	displayPrintf(DISPLAY_ROW_BTADDR,"%02x:%02x:%02x:%02x:%02x:%02x",bt_address->address.addr[5],
			bt_address->address.addr[4],bt_address->address.addr[3],bt_address->address.addr[2],
			bt_address->address.addr[1],bt_address->address.addr[0]);

}


void gecko_mesh_initiate_factory_reset(void)
{

	displayPrintf(DISPLAY_ROW_ACTION,"Factory Reset");

  /* if connection is open then close it before rebooting */
  if (conn_handle != 0xFF) {
    gecko_cmd_le_connection_close(conn_handle);
  }

  /* perform a factory reset by erasing PS storage. This removes all the keys and other settings
     that have been configured for this node */
  gecko_cmd_flash_ps_erase_all();
  // reboot after a small delay
  gecko_cmd_hardware_set_soft_timer(2 * 32768, TIMER_ID_FACTORY_RESET, 1);

}

void gecko_mesh_set_device_name(void)
{
	char device_name[20];
	struct gecko_msg_system_get_bt_address_rsp_t *bt_address;
	bt_address = gecko_cmd_system_get_bt_address();

	if(IsMeshLPN())
	{
		sprintf(device_name,"5823LPN %02x%02x",bt_address->address.addr[1],bt_address->address.addr[0]);
	}
	else
	{
		sprintf(device_name,"5823Friend %02x%02x",bt_address->address.addr[1],bt_address->address.addr[0]);
	}

	gecko_mesh_init_LCD_status(bt_address);

	BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_write_attribute_value(gattdb_device_name,0,NAME_LENGTH,device_name));

}


void gecko_mesh_send_onoff_request(int retrans)
{
  uint16 resp;
  uint16 delay;
  struct mesh_generic_request req;
  const uint32 transtime = 0; /* using zero transition time by default */

  req.kind = mesh_generic_request_on_off;
  req.on_off = pb0_state;

  // increment transaction ID for each request, unless it's a retransmission
  if (retrans == 0) {
	  trans_id++;
  }

  /* delay for the request is calculated so that the last request will have a zero delay and each
   * of the previous request have delay that increases in 50 ms steps. For example, when using three
   * on/off requests per button press the delays are set as 100, 50, 0 ms
   */
  delay = (request_count - 1) * 50;

  resp = mesh_lib_generic_client_publish(
    MESH_GENERIC_ON_OFF_CLIENT_MODEL_ID,
    elem_index,
	trans_id,
    &req,
    transtime,   // transition time in ms
    delay,
    0     // flags
    );

  if(resp)
  {
	  LOG_INFO("Publish error - code:%x",resp);
  }
  else
  {
	  LOG_INFO("Request sent, trid = %u, delay = %d\r\n", trans_id, delay);
  }

  /* keep track of how many requests has been sent */
  if (request_count > 0) {
    request_count--;
  }
}

void gecko_mesh_init_models(void)
{
	elem_index = 0;   // index of primary element is zero. This example has only one element.
	if(DeviceUsesClientModel())
	{
		LOG_INFO("Sensor client init");
		gecko_cmd_mesh_sensor_client_init();
	}
	if(DeviceUsesServerModel())
	{
		LOG_INFO("Sensor server init");
		sensor_node_init();
	}
	if(IsMeshLPN())
	{
		LOG_INFO("LPN init");
		mesh_lib_init(malloc,free,8);
		// Publish current button state
		//request_count = 3;
		//gecko_mesh_send_onoff_request(0);
		// try to initialize lpn after 30 seconds, if no configuration messages come
		BTSTACK_CHECK_RESPONSE(gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(30000),
												 TIMER_ID_NODE_CONFIGURED,
												 1));
	}
	if(IsMeshFriend())
	{
		LOG_INFO("Friend init");
		mesh_lib_init(malloc,free,9);

		//Initialize Friend functionality
		LOG_INFO("Friend mode initialization\r\n");
		uint16_t res = gecko_cmd_mesh_friend_init()->result;
		if (res)
		{
			LOG_INFO("Friend init failed 0x%x\r\n", res);
		}

		//server_state_init();

		//mesh_lib_generic_server_register_handler(MESH_GENERIC_PROPERTY_CLIENT_MODEL_ID,
		//										elem_index,
		//										server_property_request,
		//										server_property_change);

		//server_on_off_update_and_publish(elem_index,0);
	}
	displayPrintf(DISPLAY_ROW_ACTION,"Provisioned");
}


/***************************************************************************//**
 * Initialize LPN functionality with configuration and friendship establishment.
 ******************************************************************************/
void gecko_mesh_lpn_init(void)
{
  uint16 result;

  // Do not initialize LPN if lpn is currently active
  // or any GATT connection is opened
  if (lpn_active /*|| num_connections*/) {
    return;
  }

  // Initialize LPN functionality.
  result = gecko_cmd_mesh_lpn_init()->result;
  if (result) {
    printf("LPN init failed (0x%x)\r\n", result);
    return;
  }
  lpn_active = LPN_ACTIVE;
  LOG_INFO("LPN initialized\r\n");
  //DI_Print("LPN on", DI_ROW_LPN);

  // Configure the lpn with following parameters:
  // - Minimum friend queue length = 2
  // - Poll timeout = 5 seconds
  BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_lpn_config(mesh_lpn_queue_length,2));
  BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_lpn_config(mesh_lpn_poll_timeout,5 * 1000));


  printf("trying to find friend...\r\n");
  result = gecko_cmd_mesh_lpn_establish_friendship(0)->result;

  if (result != 0) {
    LOG_INFO("ret.code %x\r\n", result);
  }
}

/***************************************************************************//**
 * Deinitialize LPN functionality.
 ******************************************************************************/
void gecko_mesh_lpn_deinit(void)
{
  uint16 result;

  if (!lpn_active) {
    return; // lpn feature is currently inactive
  }

  result = gecko_cmd_hardware_set_soft_timer(0, // cancel friend finding timer
                                             TIMER_ID_FRIEND_FIND,
                                             1)->result;

  // Terminate friendship if exist
  result = gecko_cmd_mesh_lpn_terminate_friendship()->result;
  if (result) {
    printf("Friendship termination failed (0x%x)\r\n", result);
  }
  // turn off lpn feature
  result = gecko_cmd_mesh_lpn_deinit()->result;
  if (result) {
    printf("LPN deinit failed (0x%x)\r\n", result);
  }
  lpn_active = LPN_INACTIVE;
  printf("LPN deinitialized\r\n");
  //DI_Print("LPN off", DI_ROW_LPN);
}
