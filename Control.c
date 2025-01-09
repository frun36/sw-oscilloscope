#include "Control.h"
#include "ADC.h"
#include "Driver_USART.h"
#include "PIN_LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "Board_LED.h"
#include "LCDControl.h"

#include <stdio.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DEBUG


static const uint8_t CONTROL_PINS[] = {8, 12, 13, 9, 11, 10};

extern ARM_DRIVER_USART Driver_USART0;
ARM_DRIVER_USART* uart = &Driver_USART0;
Control control;

static void update_settings(uint16_t new_step, uint16_t new_vmax, uint16_t new_thresh) {
	if (new_step != control.step) {
		control.step = new_step;
		control.dt = SCOPE_MAX_X / 2 * SAMPLE_US * new_step;
	}
	
	if (control.vmax != new_vmax || new_thresh != control.thresh) {
		draw_horizontal(MIN(control.thresh * SCOPE_MAX_Y / control.vmax, SCOPE_MAX_Y), 0);
		if (new_vmax != control.vmax) {
			control.vmax = new_vmax;
			control.dv = new_vmax / 2;
		}
		control.thresh = new_thresh;
		draw_horizontal(MIN(control.thresh * SCOPE_MAX_Y / control.vmax, SCOPE_MAX_Y), 0xDD00);
	}

#ifdef DEBUG
	char buff[128] = {};
	uint32_t len = sprintf(buff, "st=%u;vm=%u;th=%u;dt=%u;dv=%u\r\n", 
		control.step, control.vmax, control.thresh, control.dt, control.dv);
	uart->Send(buff, len);
#endif
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
			if (control.step > 1)
				update_settings(control.step / 2, control.vmax, control.thresh);
			break;
		case LEFT:
			update_settings(control.step * 2, control.vmax, control.thresh);
			break;
		case TOP:
			if (control.vmax >= 300)
				update_settings(control.step, control.vmax - 300, control.thresh);
			break;
		case BOTTOM:
			update_settings(control.step, control.vmax + 300, control.thresh);
			break;
		case KEY1:
			if (control.thresh >= 300)
				update_settings(control.step, control.vmax, control.thresh - 300);
			break;
		case KEY2:
			update_settings(control.step, control.vmax, control.thresh + 300);
			break;
		default:
			break;
	}
	LED_Off(1);
	LPC_GPIOINT->IO2IntClr = 0xFFFFFFFF;
}

void init_uart() {
	// Initialize the USART driver
	uart->Initialize(NULL);
	// Power up the USART peripheral
	uart->PowerControl(ARM_POWER_FULL);
	// Configure the USART
	uart->Control(ARM_USART_MODE_ASYNCHRONOUS |
						ARM_USART_DATA_BITS_8 |
						ARM_USART_PARITY_NONE |
						ARM_USART_STOP_BITS_1 |
						ARM_USART_FLOW_CONTROL_NONE, 115200);

	// Enable (no Receiver) Transmitter
	uart->Control (ARM_USART_CONTROL_TX, 1);
	// uart->Control (ARM_USART_CONTROL_RX, 1);

	uart->Send("Helou\r\n", 7);
}
