/*
 * ble_mesh_device_type.h
 *
 *  Created on: Mar 16, 2019
 *      Author: Dan Walkes
 */

#include <stdbool.h>

#ifndef SRC_BLE_MESH_DEVICE_TYPE_H_
#define SRC_BLE_MESH_DEVICE_TYPE_H_

/**
 * Set to 1 to build an on/off publisher client model
 * Set to 0 to build an on/off subscriber server model
 */
#define DEVICE_IS_SENSOR_SERVER				0

#if DEVICE_IS_SENSOR_SERVER
#define DEVICE_USES_BLE_MESH_CLIENT_MODEL 	0
#define DEVICE_USES_BLE_MESH_SERVER_MODEL 	1
#define DEVICE_IS_BLE_MESH_LPN 				1
#define DEVICE_IS_BLE_MESH_FRIEND 			0
#else
#define DEVICE_USES_BLE_MESH_CLIENT_MODEL 	1
#define DEVICE_USES_BLE_MESH_SERVER_MODEL 	0
#define DEVICE_IS_ONOFF_SUBSCRIBER			1
#define DEVICE_IS_BLE_MESH_LPN 				0
#define DEVICE_IS_BLE_MESH_FRIEND 			1
#endif




#if DEVICE_IS_BLE_MESH_LPN
#define BUILD_INCLUDES_BLE_MESH_LPN 	1
static inline bool IsMeshLPN() { return true; }
static inline bool IsMeshFriend() { return false; }
#if DEVICE_IS_BLE_MESH_FRIEND
#error "Can't support both DEVICE_IS_BLE_MESH_LPN and DEVICE_IS_BLE_MESH_FRIEND set"
#endif
#endif

#if DEVICE_IS_BLE_MESH_FRIEND
#define BUILD_INCLUDES_BLE_MESH_LPN 	0
#define BUILD_INCLUDES_BLE_MESH_FRIEND 	1
static inline bool IsMeshLPN() { return false; }
static inline bool IsMeshFriend() { return true; }
#if DEVICE_IS_BLE_MESH_LPN
#error "Can't support both DEVICE_IS_BLE_MESH_LPN and DEVICE_IS_BLE_MESH_FRIEND set"
#endif
#endif


#if DEVICE_USES_BLE_MESH_CLIENT_MODEL
static inline bool DeviceUsesClientModel() { return true; }
#else
static inline bool DeviceUsesClientModel() { return false; }
#endif

#if DEVICE_USES_BLE_MESH_SERVER_MODEL
static inline bool DeviceUsesServerModel() { return true; }
#else
static inline bool DeviceUsesServerModel() { return false; }
#endif


#if DEVICE_IS_SENSOR_SERVER
static inline bool DeviceIsOnOffPublisher() { return true; }
#else
static inline bool DeviceIsOnOffPublisher() { return false; }
#endif

#if DEVICE_IS_ONOFF_SUBSCRIBER
static inline bool DeviceIsOnOffSubscriber() { return true; }
#else
static inline bool DeviceIsOnOffSubscriber() { return false; }
#endif

#endif /* SRC_BLE_MESH_DEVICE_TYPE_H_ */
