#include "LPC17xx.h"
#include "Board_LED.h"
#include "LCDControl.h"
#include "ADC.h"
#include "Control.h"

void setup() {
	LED_Initialize();
	LED_SetOut(0);
	
	init_lcd();
	init_tim0();
	init_adc();
	init_control();
	init_uart();
	
	LED_SetOut(1);
}

void loop() {
	handle_draw();
	LED_Off(2);
	__WFI();	
}

int main() {
	setup();
	while(1) loop();
}
