#include "Control.h"
#include "ADC.h"

#include "PIN_LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "Board_LED.h"
#include "LCDControl.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static const uint8_t CONTROL_PINS[] = {8, 12, 13, 9, 11, 10};

extern uint8_t triggered;

uint16_t control_step;
uint16_t control_vmax;
uint16_t trigger_level;

static void update_settings(uint16_t new_step, uint16_t new_vmax, uint16_t new_trigger_level) {
	control_step = new_step;
	
	if (control_vmax == new_vmax && new_trigger_level == trigger_level)
		return;
	
	draw_horizontal(MIN(trigger_level * SCOPE_MAX_Y / control_vmax, SCOPE_MAX_Y), SCOPE_MAX_X, 0);
	control_vmax = new_vmax;
	trigger_level = new_trigger_level;
	draw_horizontal(MIN(trigger_level * SCOPE_MAX_Y / control_vmax, SCOPE_MAX_Y), SCOPE_MAX_X, 0xDD00);
}

void init_control() {
	// A			P2.8
	// B			P2.12
	// C			P2.13
	// D			P2.9 !JUMPER!
	// PRESS	N.C.
	// KEY1		P2.11
	// KEY2		P2.10
	
	uint32_t int_en = 0;
	for (uint8_t i = 0; i < sizeof(CONTROL_PINS); i++) {
		PIN_Configure(2, CONTROL_PINS[i], PIN_FUNC_0, 0U, 0U);
		GPIO_SetDir(2, CONTROL_PINS[i], GPIO_DIR_INPUT);
		int_en |= (1 << CONTROL_PINS[i]); // Enable falling edge interrupt for pin
	}
	LPC_GPIOINT->IO2IntEnF = int_en;
	
	NVIC_EnableIRQ(EINT3_IRQn);
	NVIC_SetPriority(EINT3_IRQn, 0);
	
	update_settings(16, ADC_VMAX, ADC_VMAX / 2);
}

typedef enum { NONE, TOP, RIGHT, BOTTOM, LEFT, KEY1, KEY2 } ControlState;

ControlState get_joystick_state() {
	if (!GPIO_PinRead(2, CONTROL_PINS[0]))
		return TOP;
	else if (!GPIO_PinRead(2, CONTROL_PINS[1]))
		return RIGHT;
	else if (!GPIO_PinRead(2, CONTROL_PINS[2]))
		return LEFT;
	else if (!GPIO_PinRead(2, CONTROL_PINS[3]))
		return BOTTOM;
	else if (!GPIO_PinRead(2, CONTROL_PINS[4]))
		return KEY1;
	else if (!GPIO_PinRead(2, CONTROL_PINS[5]))
		return KEY2;
	else
		return NONE;
}

void EINT3_IRQHandler() {
	LED_On(1);

	for (uint32_t i = 0; i < 2000000; i++) // debounce
		;

	switch (get_joystick_state()) {
		case RIGHT:
			if (control_step > 1)
				update_settings(control_step / 2, control_vmax, trigger_level);
			break;
		case LEFT:
			update_settings(control_step * 2, control_vmax, trigger_level);
			break;
		case TOP:
			if (control_vmax >= 300)
				update_settings(control_step, control_vmax - 300, trigger_level);
			break;
		case BOTTOM:
			update_settings(control_step, control_vmax + 300, trigger_level);
			break;
		case KEY1:
			if (trigger_level >= 300)
				update_settings(control_step, control_vmax, trigger_level - 300);
			break;
		case KEY2:
			update_settings(control_step, control_vmax, trigger_level + 300);
			break;
		default:
			break;
	}
	LED_Off(1);
	LPC_GPIOINT->IO2IntClr = 0xFFFFFFFF;
}
