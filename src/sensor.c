/***************************************************************************//**
 * @file  sensor.c
 * @brief Sensor module
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

#include "sensor.h"

/***************************************************************************//**
 * @addtogroup Sensor
 * @{
 ******************************************************************************/

#define SENSOR_ELEMENT     0 ///< Sensor model located in primary element
#define NUMBER_OF_SENSORS  3 ///< Number of supported Property IDs
#define PUBLISH_ADDRESS    0 ///< The unused 0 address is used for publishing
#define IGNORED            0 ///< Parameter ignored for publishing
#define NO_FLAGS           0 ///< No flags used for message

/*******************************************************************************
 * Sensor initialization.
 * This is called at each boot if provisioning is already done.
 * Otherwise this function is called after provisioning is completed.
 ******************************************************************************/
void sensor_node_init(void)
{
  /// Descriptors of supported sensors
  /* The following properties are defined
   * 1. Ambient light level property (property ID: 0x004E)
   * 2. Ambient temperature property (property ID: 0x004F)
   * 3. Input voltage property for sensing soil moisture content (property ID: 0x0059)
   * NOTE: the properties must be ordered in ascending order by property ID
   */
  static const sensor_descriptor_t descriptors[NUMBER_OF_SENSORS] = {
    {
      .property_id = PRESENT_AMBIENT_LIGHT_LEVEL,
      .positive_tolerance = TOLERANCE_UNSPECIFIED,
      .negative_tolerance = TOLERANCE_UNSPECIFIED,
      .sampling_function = SAMPLING_INSTANTANEOUS,
      .measurement_period = MEASUREMENT_PERIOD_UNDEFINED,
      .update_interval = UPDATE_INTERVAL_UNDEFINED
    },
    {
      .property_id = PRESENT_AMBIENT_TEMPERATURE,
      .positive_tolerance = TOLERANCE_UNSPECIFIED,
      .negative_tolerance = TOLERANCE_UNSPECIFIED,
      .sampling_function = SAMPLING_INSTANTANEOUS,
      .measurement_period = MEASUREMENT_PERIOD_UNDEFINED,
      .update_interval = UPDATE_INTERVAL_UNDEFINED
    },
	{
	  .property_id = PRESENT_INPUT_VOLTAGE,
	  .positive_tolerance = TOLERANCE_UNSPECIFIED,
	  .negative_tolerance = TOLERANCE_UNSPECIFIED,
	  .sampling_function = SAMPLING_INSTANTANEOUS,
	  .measurement_period = MEASUREMENT_PERIOD_UNDEFINED,
	  .update_interval = UPDATE_INTERVAL_UNDEFINED
	}
  };
  uint16_t result = mesh_lib_sensor_server_init(SENSOR_ELEMENT,
                                                NUMBER_OF_SENSORS,
                                                descriptors);
  LOG_INFO("sensor init result %02x\r\n", result);

  // Initialize the sensors?

}

/***************************************************************************//**
 * Handling of sensor server get request event.
 * It sending sensor status message with data for all of supported Properties ID,
 * if there is no Property ID field in request. If request contains Property ID
 * that is supported, functions reply with the sensor status message with data
 * for this Property ID, in other case the message contains no data.
 *
 * @param[in] pEvt  Pointer to sensor server get request event.
 ******************************************************************************/
void handle_sensor_server_get_request(
  struct gecko_msg_mesh_sensor_server_get_request_evt_t *pEvt)
{
  LOG_INFO("evt:gecko_evt_mesh_sensor_server_get_request_id\r\n");
  uint8_t sensor_data[5];
  uint8_t len = 0;
  if ((pEvt->property_id == PRESENT_AMBIENT_LIGHT_LEVEL) || (pEvt->property_id == 0))
  {
	  illuminance_t ambient_light = light_sensor_get_illuminance();
	  len += mesh_sensor_data_to_buf(PRESENT_AMBIENT_LIGHT_LEVEL,
	                                 &sensor_data[len],
	                                 (uint8_t*)&ambient_light);
  }
  if ((pEvt->property_id == PRESENT_AMBIENT_TEMPERATURE) || (pEvt->property_id == 0))
  {
	  temperature_8_t temperature;// = get_temperature();
	  len += mesh_sensor_data_to_buf(PRESENT_AMBIENT_TEMPERATURE,
								   &sensor_data[len],
								   (uint8_t*)&temperature);
  }
  if ((pEvt->property_id == PRESENT_INPUT_VOLTAGE) || (pEvt->property_id == 0))
  {
	  present_input_voltage_t soil_voltage = soil_sensor_get_voltage();
	  len += mesh_sensor_data_to_buf(PRESENT_INPUT_VOLTAGE,
	                                 &sensor_data[len],
	                                 (uint8_t*)&soil_voltage);
  }

  if (len > 0)
  {
    gecko_cmd_mesh_sensor_server_send_status(SENSOR_ELEMENT,
                                             pEvt->client_address,
                                             pEvt->appkey_index,
                                             NO_FLAGS,
                                             len, sensor_data);
  }
  else
  {
    sensor_data[0] = pEvt->property_id & 0xFF;
    sensor_data[1] = ((pEvt->property_id) >> 8) & 0xFF;
    sensor_data[3] = 0; // Length is 0 for unsupported property_id
    gecko_cmd_mesh_sensor_server_send_status(SENSOR_ELEMENT,
                                             pEvt->client_address,
                                             pEvt->appkey_index,
                                             NO_FLAGS,
                                             3, sensor_data);
  }
}

/***************************************************************************//**
 * Handling of sensor server get column request event.
 * Used Property IDs does not have sensor series column state,
 * so reply has the same data as request according to specification.
 *
 * @param[in] pEvt  Pointer to sensor server get column request event.
 ******************************************************************************/
void handle_sensor_server_get_column_request(
  struct gecko_msg_mesh_sensor_server_get_column_request_evt_t *pEvt)
{
	LOG_INFO("evt:gecko_evt_mesh_sensor_server_get_column_request_id\r\n");
	gecko_cmd_mesh_sensor_server_send_column_status(SENSOR_ELEMENT,
                                                  pEvt->client_address,
                                                  pEvt->appkey_index,
                                                  NO_FLAGS,
                                                  pEvt->property_id,
                                                  pEvt->column_ids.len,
                                                  pEvt->column_ids.data);
}

/***************************************************************************//**
 * Handling of sensor server get series request event.
 * Used Property IDs does not have sensor series column state,
 * so reply has only Property ID according to specification.
 *
 * @param[in] pEvt  Pointer to sensor server get series request event.
 ******************************************************************************/
void handle_sensor_server_get_series_request(
  struct gecko_msg_mesh_sensor_server_get_series_request_evt_t *pEvt)
{
	LOG_INFO("evt:gecko_evt_mesh_sensor_server_get_series_request_id\r\n");
	gecko_cmd_mesh_sensor_server_send_series_status(SENSOR_ELEMENT,
                                                  pEvt->client_address,
                                                  pEvt->appkey_index,
                                                  NO_FLAGS,
                                                  pEvt->property_id,
                                                  0, NULL);
}

/***************************************************************************//**
 * Handling of sensor server publish event.
 * It is used to signal the elapse of the publish period, when the server app
 * shall publish the sensor states
 *
 * @param[in] pEvt  Pointer to sensor server publish request event
 ******************************************************************************/
void handle_sensor_server_publish_event(
  struct gecko_msg_mesh_sensor_server_publish_evt_t *pEvt)
{
  LOG_INFO("evt:gecko_evt_mesh_sensor_server_publish_id\r\n");
  uint8_t sensor_data[32];
  uint8_t len = 0;

  illuminance_t ambient_light = light_sensor_get_illuminance();
  len += mesh_sensor_data_to_buf(PRESENT_AMBIENT_LIGHT_LEVEL,
                                 &sensor_data[len],
                                 (uint8_t*)&ambient_light);

  temperature_8_t temperature;// = get_temperature();
  len += mesh_sensor_data_to_buf(PRESENT_AMBIENT_TEMPERATURE,
                                 &sensor_data[len],
                                 (uint8_t*)&temperature);

  present_input_voltage_t soil_voltage = soil_sensor_get_voltage();
  len += mesh_sensor_data_to_buf(PRESENT_INPUT_VOLTAGE,
                                 &sensor_data[len],
                                 (uint8_t*)&soil_voltage);


  if (len > 0) {
    gecko_cmd_mesh_sensor_server_send_status(SENSOR_ELEMENT,
    										 PUBLISH_ADDRESS,
                                             IGNORED,
                                             NO_FLAGS,
                                             len, sensor_data);
  }
}

/***************************************************************************//**
 * Handling of sensor server publish event.
 * It is used to signal the elapse of the publish period, when the server app
 * shall publish the sensor states
 *
 * @param[in] pEvt  Pointer to sensor server publish request event
 ******************************************************************************/
void sensor_server_publish(void)
{
  LOG_INFO("evt:gecko_evt_mesh_sensor_server_publish_id\r\n");
  uint8_t sensor_data[32];
  uint8_t len = 0;

  illuminance_t ambient_light = light_sensor_get_illuminance();
  len += mesh_sensor_data_to_buf(PRESENT_AMBIENT_LIGHT_LEVEL,
                                 &sensor_data[len],
                                 (uint8_t*)&ambient_light);

  displayPrintf(DISPLAY_ROW_LIGHTNESS,"Illuminance: %d",(uint32_t)ambient_light);

  temperature_8_t temperature;// = get_temperature();
  len += mesh_sensor_data_to_buf(PRESENT_AMBIENT_TEMPERATURE,
                                 &sensor_data[len],
                                 (uint8_t*)&temperature);

  displayPrintf(DISPLAY_ROW_TEMPVALUE,"Temp: %d C",temperature);

  present_input_voltage_t soil_voltage = soil_sensor_get_voltage();
  len += mesh_sensor_data_to_buf(PRESENT_INPUT_VOLTAGE,
                                 &sensor_data[len],
                                 (uint8_t*)&soil_voltage);

  displayPrintf(DISPLAY_ROW_SOIL,"Soil Moisture: %d",(uint16_t)soil_voltage);


  if (len > 0) {
    gecko_cmd_mesh_sensor_server_send_status(SENSOR_ELEMENT,
                                             PUBLISH_ADDRESS,
                                             IGNORED,
                                             NO_FLAGS,
                                             len, sensor_data);
  }
}

/***************************************************************************//**
 * Handling of sensor setup server get cadence request event.
 * Cadence is not supported now, so reply has only Property ID
 * according to specification.
 *
 * @param[in] pEvt  Pointer to sensor server get cadence request event.
 ******************************************************************************/
void handle_sensor_setup_server_get_cadence_request(
  struct gecko_msg_mesh_sensor_setup_server_get_cadence_request_evt_t *pEvt)
{
	LOG_INFO("evt:gecko_evt_mesh_sensor_server_get_cadence_request_id\r\n");
	gecko_cmd_mesh_sensor_setup_server_send_cadence_status(SENSOR_ELEMENT,
                                                         pEvt->client_address,
                                                         pEvt->appkey_index,
                                                         NO_FLAGS,
                                                         pEvt->property_id,
                                                         0, NULL);
}

/***************************************************************************//**
 * Handling of sensor setup server set cadence request event.
 * Cadence is not supported now, so reply has only Property ID
 * according to specification.
 *
 * @param[in] pEvt  Pointer to sensor server set cadence request event.
 ******************************************************************************/
void handle_sensor_setup_server_set_cadence_request(
  struct gecko_msg_mesh_sensor_setup_server_set_cadence_request_evt_t *pEvt)
{
	LOG_INFO("evt:gecko_evt_mesh_sensor_server_set_cadence_request_id\r\n");
	gecko_cmd_mesh_sensor_setup_server_send_cadence_status(SENSOR_ELEMENT,
                                                         pEvt->client_address,
                                                         pEvt->appkey_index,
                                                         NO_FLAGS,
                                                         pEvt->property_id,
                                                         0, NULL);
}

/***************************************************************************//**
 * Handling of sensor setup server get settings request event.
 * Settings are not supported now, so reply has only Property ID
 * according to specification.
 *
 * @param[in] pEvt  Pointer to sensor server get settings request event.
 ******************************************************************************/
void handle_sensor_setup_server_get_settings_request(
  struct gecko_msg_mesh_sensor_setup_server_get_settings_request_evt_t *pEvt)
{
	LOG_INFO("evt:gecko_evt_mesh_sensor_server_get_settings_request_id\r\n");
	gecko_cmd_mesh_sensor_setup_server_send_settings_status(SENSOR_ELEMENT,
                                                          pEvt->client_address,
                                                          pEvt->appkey_index,
                                                          NO_FLAGS,
                                                          pEvt->property_id,
                                                          0, NULL);
}

/***************************************************************************//**
 * Handling of sensor setup server get setting request event.
 * Settings are not supported now, so reply has only Property ID
 * and Sensor Property ID according to specification.
 *
 * @param[in] pEvt  Pointer to sensor server get setting request event.
 ******************************************************************************/
void handle_sensor_setup_server_get_setting_request(
  struct gecko_msg_mesh_sensor_setup_server_get_setting_request_evt_t *pEvt)
{
	LOG_INFO("evt:gecko_evt_mesh_sensor_server_get_setting_request_id\r\n");
	gecko_cmd_mesh_sensor_setup_server_send_setting_status(SENSOR_ELEMENT,
                                                         pEvt->client_address,
                                                         pEvt->appkey_index,
                                                         NO_FLAGS,
                                                         pEvt->property_id,
                                                         pEvt->setting_id,
                                                         0, NULL);
}

/***************************************************************************//**
 * Handling of sensor setup server set setting request event.
 * Settings are not supported now, so reply has only Property ID
 * and Sensor Property ID according to specification.
 *
 * @param[in] pEvt  Pointer to sensor server set setting request event.
 ******************************************************************************/
void handle_sensor_setup_server_set_setting_request(
  struct gecko_msg_mesh_sensor_setup_server_set_setting_request_evt_t *pEvt)
{
	LOG_INFO("evt:gecko_evt_mesh_sensor_server_get_setting_request_id\r\n");
	gecko_cmd_mesh_sensor_setup_server_send_setting_status(SENSOR_ELEMENT,
                                                         pEvt->client_address,
                                                         pEvt->appkey_index,
                                                         NO_FLAGS,
                                                         pEvt->property_id,
                                                         pEvt->setting_id,
                                                         0, NULL);
}

/*******************************************************************************
 *  Handling of mesh sensor server events.
 *  It handles:
 *   - sensor_server_get_request
 *   - sensor_server_get_column_request
 *   - sensor_server_get_series_request
 *   - sensor_setup_server_get_cadence_request
 *   - sensor_setup_server_set_cadence_request
 *   - sensor_setup_server_get_settings_request
 *   - sensor_setup_server_get_setting_request
 *   - sensor_setup_server_set_setting_request
 *
 *  @param[in] pEvt  Pointer to incoming sensor server event.
 ******************************************************************************/
void handle_sensor_server_events(struct gecko_cmd_packet *pEvt)
{
  switch (BGLIB_MSG_ID(pEvt->header)) {
    case gecko_evt_mesh_sensor_server_get_request_id:
      handle_sensor_server_get_request(
        &(pEvt->data.evt_mesh_sensor_server_get_request));
      break;

    case gecko_evt_mesh_sensor_server_get_column_request_id:
      handle_sensor_server_get_column_request(
        &(pEvt->data.evt_mesh_sensor_server_get_column_request));
      break;

    case gecko_evt_mesh_sensor_server_get_series_request_id:
      handle_sensor_server_get_series_request(
        &(pEvt->data.evt_mesh_sensor_server_get_series_request));
      break;

    case gecko_evt_mesh_sensor_server_publish_id:
      handle_sensor_server_publish_event(
        &(pEvt->data.evt_mesh_sensor_server_publish));
      break;

    case gecko_evt_mesh_sensor_setup_server_get_cadence_request_id:
      handle_sensor_setup_server_get_cadence_request(
        &(pEvt->data.evt_mesh_sensor_setup_server_get_cadence_request));
      break;

    case gecko_evt_mesh_sensor_setup_server_set_cadence_request_id:
      handle_sensor_setup_server_set_cadence_request(
        &(pEvt->data.evt_mesh_sensor_setup_server_set_cadence_request));
      break;

    case gecko_evt_mesh_sensor_setup_server_get_settings_request_id:
      handle_sensor_setup_server_get_settings_request(
        &(pEvt->data.evt_mesh_sensor_setup_server_get_settings_request));
      break;

    case gecko_evt_mesh_sensor_setup_server_get_setting_request_id:
      handle_sensor_setup_server_get_setting_request(
        &(pEvt->data.evt_mesh_sensor_setup_server_get_setting_request));
      break;

    case gecko_evt_mesh_sensor_setup_server_set_setting_request_id:
      handle_sensor_setup_server_set_setting_request(
        &(pEvt->data.evt_mesh_sensor_setup_server_set_setting_request));
      break;

    default:
      break;
  }
}

/** @} (end addtogroup Sensor) */
