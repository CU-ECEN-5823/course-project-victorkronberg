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

  adc_init();

  /* Infinite loop */
  while (1) {
	  // Infinite loop

	if(my_state_struct.event_bitmask!=0)
	{
		my_scheduler(&my_state_struct);
	}
	else
	{
		struct gecko_cmd_packet *evt = gecko_wait_event();
		bool pass = mesh_bgapi_listener(evt);
		if (pass)
		{
			handle_gecko_event(BGLIB_MSG_ID(evt->header), evt);
		}
	}
  };
}
