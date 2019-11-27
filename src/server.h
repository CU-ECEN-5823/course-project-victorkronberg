/*
 * client.h
 *	Based off of Silicon Labs btmesh-light example
 *  Created on: Nov 7, 2019
 *      Author: vkronber
 */

#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_

#include "main.h"

struct on_off_state
{
	uint8_t on_off_current;
	uint8_t on_off_target;
} on_off_state;

uint32_t delayed_onoff_trans;

/***************************************************************************//**
 * This function process the requests for the generic on/off model.
 *
 * @param[in] model_id       Server model ID.
 * @param[in] element_index  Server model element index.
 * @param[in] client_addr    Address of the client model which sent the message.
 * @param[in] server_addr    Address the message was sent to.
 * @param[in] appkey_index   The application key index used in encrypting the request.
 * @param[in] request        Pointer to the request structure.
 * @param[in] transition_ms  Requested transition time (in milliseconds).
 * @param[in] delay_ms       Delay time (in milliseconds).
 * @param[in] request_flags  Message flags. Bitmask of the following:
 *                           - Bit 0: Nonrelayed. If nonzero indicates
 *                                    a response to a nonrelayed request.
 *                           - Bit 1: Response required. If nonzero client
 *                                    expects a response from the server.
 ******************************************************************************/
void server_on_off_request(uint16_t model_id,
                          uint16_t element_index,
                          uint16_t client_addr,
                          uint16_t server_addr,
                          uint16_t appkey_index,
                          const struct mesh_generic_request *request,
                          uint32_t transition_ms,
                          uint16_t delay_ms,
                          uint8_t request_flags);

void server_property_request(uint16_t model_id,
                          uint16_t element_index,
                          uint16_t client_addr,
                          uint16_t server_addr,
                          uint16_t appkey_index,
                          const struct mesh_generic_request *request,
                          uint32_t transition_ms,
                          uint16_t delay_ms,
                          uint8_t request_flags);

/***************************************************************************//**
 * This function is a handler for generic on/off change event.
 *
 * @param[in] model_id       Server model ID.
 * @param[in] element_index  Server model element index.
 * @param[in] current        Pointer to current state structure.
 * @param[in] target         Pointer to target state structure.
 * @param[in] remaining_ms   Time (in milliseconds) remaining before transition
 *                           from current state to target state is complete.
 ******************************************************************************/
void server_on_off_change(uint16_t model_id,
                         uint16_t element_index,
                         const struct mesh_generic_state *current,
                         const struct mesh_generic_state *target,
                         uint32_t remaining_ms);


void server_property_change(uint16_t model_id,
                         uint16_t element_index,
                         const struct mesh_generic_state *current,
                         const struct mesh_generic_state *target,
                         uint32_t remaining_ms);

/***************************************************************************//**
 * Update generic on/off state.
 *
 * @param[in] element_index  Server model element index.
 * @param[in] remaining_ms   The remaining time in milliseconds.
 *
 * @return Status of the update operation.
 *         Returns bg_err_success (0) if succeed, non-zero otherwise.
 ******************************************************************************/
errorcode_t server_on_off_update(uint16_t element_index, uint32_t remaining_ms);

/***************************************************************************//**
 * Update generic on/off state and publish model state to the network.
 *
 * @param[in] element_index  Server model element index.
 * @param[in] remaining_ms   The remaining time in milliseconds.
 *
 * @return Status of the update and publish operation.
 *         Returns bg_err_success (0) if succeed, non-zero otherwise.
 ******************************************************************************/
errorcode_t server_on_off_update_and_publish(uint16_t element_index,
                                            uint32_t remaining_ms);

/***************************************************************************//**
 * This function is called each time the lightbulb state in RAM is changed.
 * It sets up a soft timer that will save the state in flash after small delay.
 * The purpose is to reduce amount of unnecessary flash writes.
 ******************************************************************************/
void server_on_off_state_changed(void);

/***************************************************************************//**
 * This function saves the current on-off state in Persistent Storage so that
 * the data is preserved over reboots and power cycles.
 * The light state is hold in a global variable lightbulb_state.
 * A PS key with ID 0x4004 is used to store the data.
 *
 * @return 0 if saving succeed, -1 if saving fails.
 ******************************************************************************/
int32_t server_on_off_state_store(void);

void server_property_state_changed(void);

/***************************************************************************//**
 * This function loads the saved on-off state from Persistent Storage and
 * copies the data in the global variable on_off_state.
 * If PS key with ID 0x4004 does not exist or loading failed,
 * on_off_state is set to zero and some default values are written to it.
 *
 * @return 0 if loading succeeds. -1 if loading fails.
 ******************************************************************************/
int32_t server_state_load(void);

/*******************************************************************************
 * On_off state initialization.
 * This is called at each boot if provisioning is already done.
 * Otherwise this function is called after provisioning is completed.
 ******************************************************************************/
void server_state_init(void);


#endif /* SRC_SERVER_H_ */
