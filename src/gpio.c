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

	// Set PD0 and PD1 buttons as input with no filter
	GPIO_PinModeSet(PD0_BUTTON_PORT, PD0_BUTTON_PIN, gpioModeInputPullFilter, true);
	GPIO_PinModeSet(PD1_BUTTON_PORT, PD1_BUTTON_PIN, gpioModeInputPullFilter, true);

	// Disable GPIO interrupts prior to configuring pin interrupts
	//GPIO_IntDisable(PD0_BUTTON_PIN);
	// Configure input interrupt for PD0 and PD1 buttons on rising/falling edge - enable interrupts
	GPIO_ExtIntConfig(PD0_BUTTON_PORT,PD0_BUTTON_PIN,PD0_BUTTON_PIN,true,true,true);
	GPIO_ExtIntConfig(PD1_BUTTON_PORT,PD1_BUTTON_PIN,PD1_BUTTON_PIN,true,true,true);

	GPIOINT_Init();

	GPIOINT_IrqCallbackPtr_t PD0_callback = &GPIO_PF6_IRQHandler;
	GPIOINT_IrqCallbackPtr_t PD1_callback = &GPIO_PF7_IRQHandler;


	GPIOINT_CallbackRegister(PD0_BUTTON_PIN,PD0_callback);
	GPIOINT_CallbackRegister(PD1_BUTTON_PIN,PD1_callback);

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


void GPIO_PF6_IRQHandler(uint8_t intno)
{
	// Disable interrupt nesting
	__disable_irq();

	// Acknowledge the interrupt and clear even interrupt flags
	GPIO_IntClear(0x5555);

	pb0_state = GPIO_PinInGet(PD0_BUTTON_PORT,PD0_BUTTON_PIN);
	//pb1_state = GPIO_PinInGet(PD1_BUTTON_PORT,PD1_BUTTON_PIN);

	if(IsMeshFriend() && pb0_state == 0)
	{
		gecko_external_signal(BUTTON_EVENT_MASK);
	}


	// If 0, button is pressed and GPIO pin is grounded
	if(pb0_state == 0)
	{
		LOG_INFO("PB0 pressed");
		gpioLed0SetOn();
		button_state = 0x01;
	}
	else
	{
		LOG_INFO("PB0 Released");
		gpioLed0SetOff();
		button_state = 0x00;
	}


	// Toggle LED0
	//GPIO_PinOutToggle(LED0_port, LED0_pin);

	__enable_irq();

}

void GPIO_PF7_IRQHandler(uint8_t intno)
{
	__disable_irq();

	// Acknowledge the interrupt and clear odd interrupt flags
	GPIO_IntClear(0xAAAA);

	//pb0_state = GPIO_PinInGet(PD0_BUTTON_PORT,PD0_BUTTON_PIN);
	pb1_state = GPIO_PinInGet(PD1_BUTTON_PORT,PD1_BUTTON_PIN);

	if(IsMeshFriend() && pb1_state == 0)
	{
		gecko_external_signal(PS_READ_EVENT_MASK);
	}

	if(pb1_state == 0)
	{
		LOG_INFO("PB1 pressed");
		gpioLed1SetOn();
	}
	else
	{
		LOG_INFO("PB1 released");
		gpioLed1SetOff();
	}

	__enable_irq();
}
