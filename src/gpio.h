/*
 * gpio.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include "main.h"
#include "em_gpio.h"
#include <string.h>

#define DISPLAY_ENABLE_PORT		(gpioPortD)
#define DISPLAY_ENABLE_PIN		(15)
#define DISPLAY_EXTCOMIN_PORT	(gpioPortD)
#define DISPLAY_EXTCOMIN_PIN	(13)

#define PD0_BUTTON_PORT			(gpioPortF)
#define PD0_BUTTON_PIN			(6)
#define PD0_BUTTON_PIN_MASK		(0x40)
#define PD1_BUTTON_PORT			(gpioPortF)
#define PD1_BUTTON_PIN			(7)

#define BUTTON_PRESSED			(0x00)
#define BUTTON_RELEASED			(0x01)

// GPIO related LCD display #defines
#define GPIO_SET_DISPLAY_EXT_COMIN_IMPLEMENTED 	(1)
#define GPIO_DISPLAY_SUPPORT_IMPLEMENTED		(1)

uint8_t button_state;
uint8_t pb0_state;
uint8_t pb1_state;

/**
 * @description:  Initialize GPIO pins for LED0 and 1 and initializes interrupts
 *                on rising/falling edge of PB0
 */
void gpioInit();

/**
 * @description:  Turn on LED0
 */
void gpioLed0SetOn();

/**
 * @description:  Turn off LED0
 */
void gpioLed0SetOff();

/**
 * @description:  Turn on LED1
 */
void gpioLed1SetOn();

/**
 * @description:  Turn off LED1
 */
void gpioLed1SetOff();

/**
 * [gpioEnableDisplay]
 * @description: Enables power to the LCD display.  Disables power load management
 *               for Si7021 as they share the same enable pin
 */
void gpioEnableDisplay(void);

/**
 * [gpioSetDisplayExtcomin]
 * @description: Toggles Extcomin pin on LCD to prevent charge buildup in cell
 * @param        high [description]
 */
void gpioSetDisplayExtcomin(bool high);

/**
 * [gpio_get_button_state]
 * @description:  Returns the current state of PB0 to external functions
 * @return        [Sets state of pb0 button press
 *                Pressed     = 0x01
 *                Not pressed = 0x00]
 */
void gpio_get_button_state(void);
#endif /* SRC_GPIO_H_ */
