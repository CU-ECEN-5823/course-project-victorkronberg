/*
 * gpio.c
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */
#include "gpio.h"


#define	LED0_port gpioPortF
#define LED0_pin	4
#define LED1_port gpioPortF
#define LED1_pin 5

void gpioInit()
{
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateStrong);
	//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateStrong);
	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);
}

void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}
void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}
void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}
void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
}

void gpioEnableDisplay(void)
{
	GPIO_PinOutSet(DISPLAY_ENABLE_PORT, DISPLAY_ENABLE_PIN);
}

void gpioSetDisplayExtcomin(bool high)
{
	if(high)
	{
		GPIO_PinOutClear(DISPLAY_EXTCOMIN_PORT,DISPLAY_EXTCOMIN_PIN);
	}
	else
	{
		GPIO_PinOutSet(DISPLAY_EXTCOMIN_PORT,DISPLAY_EXTCOMIN_PIN);
	}
}

void gpio_get_button_state(void)
{
	pb0_state = GPIO_PinInGet(PD0_BUTTON_PORT,PD0_BUTTON_PIN);
}


void GPIO_EVEN_IRQHandler(void)
{
	// Disable interrupt nesting
	__disable_irq();

	// Acknowledge the interrupt and clear even interrupt flags
	GPIO_IntClear(0x5555);

	pb0_state = GPIO_PinInGet(PD0_BUTTON_PORT,PD0_BUTTON_PIN);
	pb1_state = GPIO_PinInGet(PD1_BUTTON_PORT,PD1_BUTTON_PIN);

	if(DEVICE_IS_ONOFF_PUBLISHER)
	{
		//gecko_external_signal(BUTTON_EVENT_MASK);
	}

	LOG_INFO("GPIO Pin0 state is %d",pb0_state);
	LOG_INFO("GPIO Pin1 state is %d",pb1_state);

	// If 0, button is pressed and GPIO pin is grounded
	if(pb0_state == 0)
	{
		gpioLed0SetOn();
		button_state = 0x01;
	}
	else
	{
		gpioLed0SetOff();
		button_state = 0x00;
	}


	// Toggle LED0
	//GPIO_PinOutToggle(LED0_port, LED0_pin);

	__enable_irq();

}

void GPIO_ODD_IRQHandler(void)
{
	__disable_irq();

	// Acknowledge the interrupt and clear odd interrupt flags
	GPIO_IntClear(0xAAAA);

	pb0_state = GPIO_PinInGet(PD0_BUTTON_PORT,PD0_BUTTON_PIN);
	pb1_state = GPIO_PinInGet(PD1_BUTTON_PORT,PD1_BUTTON_PIN);

	LOG_INFO("GPIO Pin0 state is %d",pb0_state);
	LOG_INFO("GPIO Pin1 state is %d",pb1_state);

	if(pb1_state == 0)
	{
		gpioLed1SetOn();
	}
	else
	{
		gpioLed1SetOff();
	}

	__enable_irq();
}
