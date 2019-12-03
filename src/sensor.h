/***************************************************************************//**
 * @file  sensor.h
 * @brief Sensor header file
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

#ifndef SENSOR_H
#define SENSOR_H

/* C Standard Library headers */
#include <stdio.h>

/* Bluetooth stack headers */
#include "native_gecko.h"
#include "mesh_sensor.h"

#include "main.h"

/***************************************************************************//**
 * @defgroup Sensor Sensor Module
 * @brief Sensor Module Implementation
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Sensor
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * Sensor initialization.
 * This is called at each boot if provisioning is already done.
 * Otherwise this function is called after provisioning is completed.
 ******************************************************************************/
void sensor_node_init(void);

/***************************************************************************//**
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
void handle_sensor_server_events(struct gecko_cmd_packet *pEvt);

/***************************************************************************//**
 * Handling of sensor server publish event.
 * It is used to signal the elapse of the publish period, when the server app
 * shall publish the sensor states
 *
 * @param[in] pEvt  Pointer to sensor server publish request event
 ******************************************************************************/
void handle_sensor_server_publish_event(struct gecko_msg_mesh_sensor_server_publish_evt_t *pEvt);

void sensor_server_publish(void);

/** @} (end addtogroup Sensor) */

#endif /* SENSOR_H */
