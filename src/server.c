/*
 * client.c
 * These functions are for interfacing with the Silicon Labs BL Mesh API for
 * server devices.
 * 
 *  Created on: Nov 7, 2019
 *      Author: vkronber
 */


#include <src/server.h>

void server_on_off_request(uint16_t model_id,
                          uint16_t element_index,
                          uint16_t client_addr,
                          uint16_t server_addr,
                          uint16_t appkey_index,
                          const struct mesh_generic_request *request,
                          uint32_t transition_ms,
                          uint16_t delay_ms,
                          uint8_t request_flags)
{
  LOG_INFO("ON/OFF request: requested state=<%s>, transition=%lu, delay=%u\r\n",
         request->on_off ? "ON" : "OFF", transition_ms, delay_ms);


	LOG_INFO("State changing to <%s>\r\n", request->on_off ? "ON" : "OFF");

	on_off_state.on_off_current = request->on_off;
	on_off_state.on_off_target = request->on_off;


	server_on_off_state_changed();

  if(on_off_state.on_off_current == BUTTON_PRESSED)
  {
	  displayPrintf(DISPLAY_ROW_ACTION,"Button Pressed");
  }
  else
  {
	  displayPrintf(DISPLAY_ROW_ACTION,"Button Released");
  }


}

void server_property_request(uint16_t model_id,
                          uint16_t element_index,
                          uint16_t client_addr,
                          uint16_t server_addr,
                          uint16_t appkey_index,
                          const struct mesh_generic_request *request,
                          uint32_t transition_ms,
                          uint16_t delay_ms,
                          uint8_t request_flags)
{

	LOG_INFO("State changing to <%s>\r\n", request->on_off ? "ON" : "OFF");

	for (uint32_t i = 0; i < request->property.length ; i++)
	{
		sensor_data.buffer[i] = *(request->property.buffer + i);
	}

	server_property_state_changed();

	displayPrintf(DISPLAY_ROW_ACTION,"Light: %d mV",sensor_data.data.lightness);
	displayPrintf(DISPLAY_ROW_ACTION,"Soil: %d mV",sensor_data.data.soil_moisture);

}


void server_on_off_change(uint16_t model_id,
                         uint16_t element_index,
                         const struct mesh_generic_state *current,
                         const struct mesh_generic_state *target,
                         uint32_t remaining_ms)
{

    LOG_INFO("on-off state changed %u to %u\r\n", on_off_state.on_off_current, current->on_off.on);

    on_off_state.on_off_current = current->on_off.on;
    server_on_off_state_changed();

    if(on_off_state.on_off_current == BUTTON_PRESSED)
	{
    	displayPrintf(DISPLAY_ROW_ACTION,"Button Pressed");
	}
	else
	{
		displayPrintf(DISPLAY_ROW_ACTION,"Button Released");
	}

}

void server_property_change(uint16_t model_id,
                         uint16_t element_index,
                         const struct mesh_generic_state *current,
                         const struct mesh_generic_state *target,
                         uint32_t remaining_ms)
{

    LOG_INFO("Property state changed %u to %u\r\n", on_off_state.on_off_current, current->on_off.on);

    for (uint32_t i = 0; i < current->property.length ; i++)
	{
		sensor_data.buffer[i] = *(current->property.buffer + i);
	}

	server_property_state_changed();

	displayPrintf(DISPLAY_ROW_ACTION,"Light: %d mV",sensor_data.data.lightness);
	displayPrintf(DISPLAY_ROW_ACTION,"Soil: %d mV",sensor_data.data.soil_moisture);

}


errorcode_t server_on_off_update(uint16_t element_index, uint32_t remaining_ms)
{
  struct mesh_generic_state current, target;

  current.kind = mesh_generic_state_on_off;
  current.on_off.on = on_off_state.on_off_current;

  target.kind = mesh_generic_state_on_off;
  target.on_off.on = on_off_state.on_off_target;

  return mesh_lib_generic_server_update(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,
                                        element_index,
                                        &current,
                                        &target,
                                        remaining_ms);
}


errorcode_t server_on_off_update_and_publish(uint16_t element_index,
                                            uint32_t remaining_ms)
{
  errorcode_t errorcode;

  errorcode = server_on_off_update(element_index, remaining_ms);
  if (errorcode == bg_err_success) {
	  errorcode = mesh_lib_generic_server_publish(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,
                                        element_index,
                                        mesh_generic_state_on_off);
  }

  return errorcode;
}


void server_on_off_state_changed(void)
{
  gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(5000), TIMER_ID_SAVE_STATE, 1);
}

void server_property_state_changed(void)
{
  //gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(5000), TIMER_ID_SAVE_STATE, 1);
}


int32_t server_on_off_state_store(void)
{
  struct gecko_msg_flash_ps_save_rsp_t* pSave;

  pSave = gecko_cmd_flash_ps_save(0x4004, sizeof(struct on_off_state), (const uint8*)&on_off_state);

  if (pSave->result) {
    BTSTACK_CHECK_RESPONSE(pSave);
    return(-1);
  }

  return 0;
}


int32_t server_state_load(void)
{
  struct gecko_msg_flash_ps_load_rsp_t* pLoad;

  pLoad = gecko_cmd_flash_ps_load(0x4004);

  // Set default values if ps_load fail or size of lightbulb_state has changed
  if (pLoad->result || (pLoad->value.len != sizeof(struct on_off_state))) {
    memset(&on_off_state, 0, sizeof(struct on_off_state));
    on_off_state.on_off_current = BUTTON_RELEASED;
    on_off_state.on_off_target = BUTTON_RELEASED;
    return -1;
  }

  memcpy(&on_off_state, pLoad->value.data, pLoad->value.len);

  return 0;
}


void server_state_init(void)
{
	memset(&on_off_state, BUTTON_RELEASED, sizeof(struct on_off_state));
	if (server_state_load() != 0) {
		LOG_INFO("Server_state_load() failed, using defaults\r\n");
	}
}
