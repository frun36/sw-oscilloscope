#include "LPC17xx.h"
#include "Board_LED.h"
#include "LCDControl.h"

#include <math.h>

uint16_t buff[SCOPE_MAX_X] = {};

void setup() {
	init_lcd();
	LED_Initialize();
	LED_SetOut(0);
	fill(LCDBlack);
	
	for (uint32_t i = 0; i < SCOPE_MAX_X; i++) {
		float sine = sinf(2 * 3.1415 / SCOPE_MAX_X * i);
		buff[i] = (sine + 1.) * 0.5 * SCOPE_MAX_Y;
	}
}

void loop() {
	draw_buffer(buff, 0xffff);
}

int main() {
	setup();
	while(1) loop();
}
