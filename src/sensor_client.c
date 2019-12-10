/***************************************************************************//**
 * @file  sensor_client.c
 * @brief Sensor client module
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

#include "sensor_client.h"

/***************************************************************************//**
 * @addtogroup SensorClient
 * @{
 ******************************************************************************/

#define SENSOR_ELEMENT        0 ///< Sensor client model located in primary element
#define PUBLISH_ADDRESS       0 ///< The unused 0 address is used for publishing
#define IGNORED               0 ///< Parameter ignored for publishing
#define NO_FLAGS              0 ///< No flags used for message
#define SIZE_OF_DESCRIPTOR    8 ///< The size of descriptor is 8 bytes
#define DISPLAYED_SENSORS     1 ///< There is place for 5 sensors on the display
#define PROPERTIES_NUMBER     2 ///< Number of supported properties
#define PROPERTY_ID_SIZE      2 ///< Size of property ID in bytes
#define PROPERTY_HEADER_SIZE  3 ///< Size of property header in bytes

/// Counter for devices registered for displaying sensor data
static uint8_t registered_devices = 0;
/// Table that saves addresses of displayed sensors
static uint16_t address_table[DISPLAYED_SENSORS];
/// Currently displayed property ID
static uint8_t current_property = 0;
/// Property IDs supported by application
static const mesh_device_properties_t properties[PROPERTIES_NUMBER] = {
  //PRESENT_AMBIENT_TEMPERATURE,
  PRESENT_INPUT_VOLTAGE,
  PRESENT_AMBIENT_LIGHT_LEVEL
};

/*******************************************************************************
 * It changes currently displayed property ID.
 ******************************************************************************/
void sensor_client_change_property(void)
{
  current_property++;
  if (current_property == PROPERTIES_NUMBER)
  {
    current_property = 0;
  }

  LOG_INFO("New property ID is %4.4x\r\n", properties[current_property]);
}

/*******************************************************************************
 * Publishing of sensor client get descriptor request for currently displayed
 * property id. It also resets the registered devices counter.
 ******************************************************************************/
void sensor_client_publish_get_descriptor_request(void)
{
  registered_devices = 0;
  for (uint8_t sensor = 0; sensor < DISPLAYED_SENSORS; sensor++) {
    //DI_Print("", DI_ROW_SENSOR_DATA + sensor);
  }
  LOG_INFO("get descriptor request");
  BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_sensor_client_get_descriptor(SENSOR_ELEMENT,
                                              PUBLISH_ADDRESS,
                                              IGNORED,
                                              NO_FLAGS,
                                              properties[current_property]));
}

/***************************************************************************//**
 * Handling of sensor client descriptor status event.
 *
 * @param[in] pEvt  Pointer to sensor client descriptor status event.
 ******************************************************************************/
void handle_sensor_client_descriptor_status(
  struct gecko_msg_mesh_sensor_client_descriptor_status_evt_t *pEvt)
{
  printf("evt:gecko_evt_mesh_sensor_client_descriptor_status_id\r\n");

  sensor_descriptor_t descriptor;
  if (pEvt->descriptors.len >= SIZE_OF_DESCRIPTOR) {
    mesh_lib_sensor_descriptors_from_buf(&descriptor,
                                         pEvt->descriptors.data,
                                         SIZE_OF_DESCRIPTOR);
    //descriptor.property_id == PRESENT_AMBIENT_TEMPERATURE //properties[current_property]
    if (registered_devices < DISPLAYED_SENSORS) {
      address_table[registered_devices] = pEvt->server_address;
      registered_devices++;
    }
  }
}

/*******************************************************************************
 * Publishing of sensor client get request for currently displayed property id.
 ******************************************************************************/
void sensor_client_publish_get_request(void)
{
	LOG_INFO("publish get request");
	BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_sensor_client_get(SENSOR_ELEMENT,
                                   PUBLISH_ADDRESS,
                                   IGNORED,
                                   NO_FLAGS,
								   0) );
}

/***************************************************************************//**
 * Handling of sensor client status event.
 *
 * @param[in] pEvt  Pointer to sensor client status event.
 ******************************************************************************/
void handle_sensor_client_status(
  struct gecko_msg_mesh_sensor_client_status_evt_t *pEvt)
{
  LOG_INFO("evt:gecko_evt_mesh_sensor_client_status_id\r\n");
  struct sensor_struct sensors;
  uint8_t *sensor_data = pEvt->sensor_data.data;
  uint8_t data_len = pEvt->sensor_data.len;
  uint8_t pos = 0;
  for (uint8_t sensor = 0; sensor < DISPLAYED_SENSORS; sensor++) {
    //if (pEvt->server_address == address_table[sensor]) {
      while (pos < data_len) {
        if (data_len - pos > PROPERTY_ID_SIZE) {
          mesh_device_properties_t property_id = (mesh_device_properties_t)(sensor_data[pos] + (sensor_data[pos + 1] << 8));
          uint8_t property_len = sensor_data[pos + PROPERTY_ID_SIZE];
          LOG_INFO("len: %d ID1: %04x proplen: %d",data_len,property_id,property_len);
          uint8_t *property_data = NULL;
          if (property_len && (data_len - pos > PROPERTY_HEADER_SIZE)) {
            property_data = &sensor_data[pos + PROPERTY_HEADER_SIZE];
          }
          //if (property_id == properties[current_property]) {
            char tmp[21];
            switch (property_id) {
              case PRESENT_INPUT_VOLTAGE:
                if (property_len == 2)
                {
                  mesh_device_property_t property = mesh_sensor_data_from_buf(PRESENT_INPUT_VOLTAGE, property_data);
                  present_input_voltage_t voltage = property.voltage;
                  sensors.soil_moisture = voltage;
                  LOG_INFO("Input Voltage %d",property.voltage);
                  displayPrintf(DISPLAY_ROW_SOIL,"Mstr: %d.%02d %%",(voltage / 50),(voltage * 2) % 100);
                }
                else
                {
                	displayPrintf(DISPLAY_ROW_SOIL,"Mstr: N/A");
                }

                break;

              case PRESENT_AMBIENT_TEMPERATURE:
				if (property_len == 1) {
				  mesh_device_property_t property = mesh_sensor_data_from_buf(PRESENT_AMBIENT_TEMPERATURE, property_data);
				  temperature_8_t temperature = property.temperature_8;
				  sensors.temperature = temperature;
				  displayPrintf(DISPLAY_ROW_TEMPVALUE,"Temp: %3d.%1d C",temperature / 2, (temperature * 5) % 10);
				}
				else
				{
					displayPrintf(DISPLAY_ROW_TEMPVALUE,"Temp: N/A");
				}
				break;
              case PRESENT_AMBIENT_LIGHT_LEVEL:
				if (property_len == 3)
				{
					mesh_device_property_t property = mesh_sensor_data_from_buf(PRESENT_AMBIENT_LIGHT_LEVEL, property_data);
					illuminance_t illuminance = property.illuminance;
					sensors.illuminance = illuminance;
					displayPrintf(DISPLAY_ROW_LIGHTNESS,"Illum: %d.%02d %%",illuminance / 50,(illuminance * 2) % 100);
				}
				else
				{
					displayPrintf(DISPLAY_ROW_LIGHTNESS,"Illum: N/A");
				}
				break;

              default:
                break;
            }
            pos += PROPERTY_HEADER_SIZE + property_len;
          }

        }
      	  // else {
         // pos = data_len;
        //}
      //}
    //}
  }
  sensors.timestamp = timerGetRunTimeMilliseconds();

  persistent_storage_save(sensors);

}

/*******************************************************************************
 * Handling of mesh sensor client events.
 * It handles:
 *  - sensor_client_descriptor_status
 *  - sensor_client_status
 *
 * @param[in] pEvt  Pointer to incoming sensor server event.
 ******************************************************************************/
void handle_sensor_client_events(struct gecko_cmd_packet *pEvt)
{
  switch (BGLIB_MSG_ID(pEvt->header)) {
    case gecko_evt_mesh_sensor_client_descriptor_status_id:
      handle_sensor_client_descriptor_status(
        &(pEvt->data.evt_mesh_sensor_client_descriptor_status));
      break;

    case gecko_evt_mesh_sensor_client_status_id:
      handle_sensor_client_status(
        &(pEvt->data.evt_mesh_sensor_client_status));
      break;

    case gecko_evt_mesh_sensor_client_publish_id:
	  LOG_INFO("evt:gecko_evt_mesh_sensor_client_status_id\r\n");
	  break;

    default:
      break;
  }
}

/** @} (end addtogroup Sensor) */
