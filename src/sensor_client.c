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
#define DISPLAYED_SENSORS     5 ///< There is place for 5 sensors on the display
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
  PRESENT_AMBIENT_TEMPERATURE,
  PEOPLE_COUNT
};

/*******************************************************************************
 * It changes currently displayed property ID.
 ******************************************************************************/
void sensor_client_change_property(void)
{
  current_property++;
  if (current_property == PROPERTIES_NUMBER) {
    current_property = 0;
  }
  for (uint8_t sensor = 0; sensor < DISPLAYED_SENSORS; sensor++) {
    //DI_Print("", DI_ROW_SENSOR_DATA + sensor);
  }
  printf("New property ID is %4.4x\r\n", properties[current_property]);
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
  gecko_cmd_mesh_sensor_client_get_descriptor(SENSOR_ELEMENT,
                                              PUBLISH_ADDRESS,
                                              IGNORED,
                                              NO_FLAGS,
                                              properties[current_property]);
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
    if (descriptor.property_id == properties[current_property]
        && registered_devices < DISPLAYED_SENSORS) {
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
  gecko_cmd_mesh_sensor_client_get(SENSOR_ELEMENT,
                                   PUBLISH_ADDRESS,
                                   IGNORED,
                                   NO_FLAGS,
                                   properties[current_property]);
}

/***************************************************************************//**
 * Handling of sensor client status event.
 *
 * @param[in] pEvt  Pointer to sensor client status event.
 ******************************************************************************/
void handle_sensor_client_status(
  struct gecko_msg_mesh_sensor_client_status_evt_t *pEvt)
{
  printf("evt:gecko_evt_mesh_sensor_client_status_id\r\n");
  uint8_t *sensor_data = pEvt->sensor_data.data;
  uint8_t data_len = pEvt->sensor_data.len;
  uint8_t pos = 0;
  for (uint8_t sensor = 0; sensor < DISPLAYED_SENSORS; sensor++) {
    if (pEvt->server_address == address_table[sensor]) {
      while (pos < data_len) {
        if (data_len - pos > PROPERTY_ID_SIZE) {
          mesh_device_properties_t property_id = (mesh_device_properties_t)(sensor_data[pos] + (sensor_data[pos + 1] << 8));
          uint8_t property_len = sensor_data[pos + PROPERTY_ID_SIZE];
          uint8_t *property_data = NULL;
          if (property_len && (data_len - pos > PROPERTY_HEADER_SIZE)) {
            property_data = &sensor_data[pos + PROPERTY_HEADER_SIZE];
          }
          if (property_id == properties[current_property]) {
            char tmp[21];
            switch (property_id) {
              case PRESENT_INPUT_VOLTAGE:
                if (property_len == 2) {
                  mesh_device_property_t property = mesh_sensor_data_from_buf(PRESENT_INPUT_VOLTAGE, property_data);
                  count16_t voltage = property.voltage;
                  if (voltage == (voltage_t)0xFFFF) {
                    snprintf(tmp, 21, "Adr %4x V   N/K", address_table[sensor]);
                  } else {
                    snprintf(tmp, 21, "Adr %4x V %5u", address_table[sensor], voltage);
                  }
                } else {
                  snprintf(tmp, 21, "Adr %4x V   N/A", address_table[sensor]);
                }
                displayPrintf(DISPLAY_ROW_SOIL,tmp);
                break;

              case PRESENT_AMBIENT_TEMPERATURE:
				if (property_len == 1) {
				  mesh_device_property_t property = mesh_sensor_data_from_buf(PRESENT_AMBIENT_TEMPERATURE, property_data);
				  temperature_8_t temperature = property.temperature_8;

				  if (temperature == (temperature_8_t)0xFF)
				  {
					snprintf(tmp, 21, "Adr %4x Temp    N/K", address_table[sensor]);
				  }
				  else
				  {
					snprintf(tmp, 21, "Adr %4x Temp %3d.%1dC", address_table[sensor], temperature / 2, (temperature * 5) % 10);
				  }
				}
				else
				{
				  snprintf(tmp, 21, "Adr %4x Temp    N/A", address_table[sensor]);
				}
				displayPrintf(DISPLAY_ROW_TEMPVALUE,tmp);
				break;
              case PRESENT_AMBIENT_LIGHT_LEVEL:
				if (property_len == 1)
				{
					mesh_device_property_t property = mesh_sensor_data_from_buf(PRESENT_AMBIENT_LIGHT_LEVEL, property_data);
					illuminance_t illuminance = property.illuminance;

					if (illuminance == (illuminance_t)0xFF)
					{
					  snprintf(tmp, 21, "Adr %4x Temp    N/K", address_table[sensor]);
					}
					else
					{
					  snprintf(tmp, 21, "Adr %4x Illum %3d.%1dC", address_table[sensor], illuminance / 2, (illuminance * 5) % 10);
					}
				}
				else
				{
				snprintf(tmp, 21, "Adr %4x Temp    N/A", address_table[sensor]);
				}
				displayPrintf(DISPLAY_ROW_LIGHTNESS,tmp);
				break;

              default:
                break;
            }
          }
          pos += PROPERTY_HEADER_SIZE + property_len;
        } else {
          pos = data_len;
        }
      }
    }
  }
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

    default:
      break;
  }
}

/** @} (end addtogroup Sensor) */
