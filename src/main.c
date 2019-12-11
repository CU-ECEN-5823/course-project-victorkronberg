/**
 * main.c
 * Main function
 *
 *    Created on: Nov 11, 2019
 *        Author: Victor Kronberg
 */

#include "main.h"

extern void gecko_main_init();
bool mesh_bgapi_listener(struct gecko_cmd_packet *evt);
extern void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt);

int main(void)
{

  sample = 0;
  millivolts = 0;
  // Initialize stack
  gecko_main_init();

  logInit();

  gpioInit();

  init_letimer();

  init_scheduler();

  displayInit();

  i2c_init();

  adc_init();

  struct gecko_cmd_packet *evt;

  /* Infinite loop */
  while (1) {
	  // Infinite loop

	if(my_state_struct.event_bitmask!=0)
	{
		if( IsMeshLPN() )
		{
			my_scheduler(&my_state_struct);
		}
		else if ( IsMeshFriend() )
		{
			client_scheduler(&my_state_struct);
		}

	}
	else
	{
		// If there are no events pending then the next call to gecko_wait_event()
		// may cause device go to deep sleep.
		// Make sure that debug prints are flushed before going to sleep
		//if (!gecko_event_pending()) {
		//  RETARGET_SerialFlush();
		//}

		evt = gecko_wait_event();
		bool pass = mesh_bgapi_listener(evt);
		if (pass)
		{
			handle_gecko_event(BGLIB_MSG_ID(evt->header), evt);
		}
	}
  };
}
