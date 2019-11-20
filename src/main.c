#include "main.h"

extern void gecko_main_init();
bool mesh_bgapi_listener(struct gecko_cmd_packet *evt);
extern void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt);

volatile uint32_t sample;
volatile uint32_t millivolts;

int main(void)
{

  // Initialize stack
  gecko_main_init();

  init_letimer();

  gpioInit();

  adc_init();

  logInit();

  init_scheduler();

  /* Infinite loop */
  while (1) {
	  // Infinite loop

	struct gecko_cmd_packet *evt = gecko_wait_event();
	bool pass = mesh_bgapi_listener(evt);
	if (pass) {
		handle_gecko_event(BGLIB_MSG_ID(evt->header), evt);
	}
  };
}
