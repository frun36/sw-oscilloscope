#include "Control.h"
#include "ADC.h"

#include "PIN_LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "Board_LED.h"

static const uint8_t JOYSTICK_PINS[] = {8, 12, 13, 11, 10};

uint8_t control_step = 1;
uint16_t control_vmax = ADC_VMAX;

void init_joystick() {
	// A			P2.8
	// B			P2.12
	// C			P2.13
	// D			P2.11 !JUMPER!
	// PRESS	P2.10 !JUMPER!
	
	uint32_t int_en = 0;
	for (uint8_t i = 0; i < 5; i++) {
		PIN_Configure(2, JOYSTICK_PINS[i], PIN_FUNC_0, 0U, 0U);
		GPIO_SetDir(2, JOYSTICK_PINS[i], GPIO_DIR_INPUT);
		int_en |= (1 << JOYSTICK_PINS[i]); // Enable falling edge interrupt for pin
	}
	LPC_GPIOINT->IO2IntEnF = int_en;
	
	NVIC_EnableIRQ(EINT3_IRQn);
	NVIC_SetPriority(EINT3_IRQn, 0);
}

typedef enum { CENTER, TOP, RIGHT, BOTTOM, LEFT, PRESSED } JoystickState;

JoystickState get_joystick_state() {
	if (!GPIO_PinRead(2, JOYSTICK_PINS[0]))
		return TOP;
	else if (!GPIO_PinRead(2, JOYSTICK_PINS[1]))
		return LEFT;
	else if (!GPIO_PinRead(2, JOYSTICK_PINS[2]))
		return BOTTOM;
	else if (!GPIO_PinRead(2, JOYSTICK_PINS[3]))
		return RIGHT;
	else if (!GPIO_PinRead(2, JOYSTICK_PINS[4]))
		return PRESSED;
	else
		return CENTER;
}

void EINT3_IRQHandler() {
	LED_On(2);

	for (uint32_t i = 0; i < 1000000; i++) // debounce
		;

	switch (get_joystick_state()) {
		case LEFT:
			if (control_step > 1)
				control_step--;
			break;
		case RIGHT:
			control_step++;
			break;
		case TOP:
			if (control_vmax <= ADC_VMAX - 100)
				control_vmax += 100;
			break;
		case BOTTOM:
			if (control_vmax >= 100)
				control_vmax -= 100;
			break;
	}
	
	LED_Off(2);
	LPC_GPIOINT->IO2IntClr = 0xFFFFFFFF;
}
