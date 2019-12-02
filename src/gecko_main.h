/*
 * gecko_main.h
 *
 *  Created on: Nov 17, 2019
 *      Author: vkronber
 */

#ifndef SRC_GECKO_MAIN_H_
#define SRC_GECKO_MAIN_H_

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
#include "src/ble_mesh_device_type.h"

#include "main.h"

/*******************************************************************************
 * Provisioning bearers defines.
 ******************************************************************************/
#define PB_ADV   0x1 ///< Advertising Provisioning Bearer
#define PB_GATT  0x2 ///< GATT Provisioning Bearer

void gecko_bgapi_classes_init_friend(void);
void gecko_bgapi_classes_init_lpn(void);
void gecko_main_init(void);
void handle_node_initialized_event(struct gecko_msg_mesh_node_initialized_evt_t *pEvt);
void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt);


#endif /* SRC_GECKO_MAIN_H_ */
