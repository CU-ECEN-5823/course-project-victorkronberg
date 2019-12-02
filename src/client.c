/*
 * client.c
 *
 *  Created on: Nov 23, 2019
 *      Author: vkronber
 */


#include "client.h"


/***************************************************************************//**
 * This function publishes the latest sensor data acquired from the LPN's sensors.
 * Global struct sensor_data holds the latest sensor information.
 *
 * param[in] retrans  Indicates if this is the first request or a retransmission,
 *                    possible values are 0 = first request, 1 = retransmission.
 ******************************************************************************/
void client_publish_state(uint32_t retrans)
{
  uint16 resp;
  uint16 delay;
  struct mesh_generic_request req;
  const uint32 transtime = 0; /* using zero transition time by default */

  req.kind = mesh_generic_request_property_user;
  req.property.id = 0xffff;
  req.property.length = sizeof(sensor_data);
  req.property.buffer = sensor_data.buffer;

  // increment transaction ID for each request, unless it's a retransmission
  if (retrans == 0)
  {
	  trans_id++;
  }

  delay = 0;

  resp = mesh_lib_generic_client_publish(
	MESH_GENERIC_PROPERTY_CLIENT_MODEL_ID,
	elem_index,
	trans_id,
    &req,
	transtime,     // transition
    delay,
    0     // flags
    );

  if (resp) {
    LOG_INFO("gecko_cmd_mesh_generic_client_publish failed,code %x\r\n", resp);
  } else {
    LOG_INFO("request sent, trid = %u, delay = %d\r\n", trans_id, delay);
  }
}
