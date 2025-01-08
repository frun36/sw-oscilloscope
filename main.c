#include "LPC17xx.h"
#include "Board_LED.h"
#include "LCDControl.h"
#include "ADC.h"
#include "Control.h"

Buffer buff;
uint8_t do_draw = 0;

void init_tim0() {
	// uint32_t us = 100; // 10 kHz sample rate
	uint32_t us = 10;
	LPC_TIM0->TCR = 0;
	LPC_TIM0->TCR = 1;
	LPC_TIM0->PR = 0;
	LPC_TIM0->MR0 = SystemCoreClock / (8000000 / us);
	LPC_TIM0->MCR = 0b11;
	LPC_TIM0->EMR |= (0b11 << 6); // When match occurs, toggle MAT0.1
}
	
void setup() {
	LED_Initialize();
	LED_SetOut(0);
	
	init_lcd();
	init_tim0();
	init_adc();
	init_control();
	
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
