#include "LPC17xx.h"
#include "Board_LED.h"
#include "LCDControl.h"

uint16_t buff[SCOPE_MAX_X] = {};

void init_adc() {
	LPC_SC->PCONP |= (1 << 12); 			// Enable ADC
	LPC_ADC->ADCR = (1 << 1) 			| 	// ADC0.1
									(1 << 21) 		| 	// ADC Operational
									(5 << 8)  		| 	// Divide clock to 5 Mhz
									(0b100 << 24);  	// Start conversion on MAT0.1 rising edge (default, bit 27)
	
	LPC_PINCON->PINSEL1 |= (1 << 16); // P0.24 as ADC0.1
}

void init_tim0() {
	uint32_t us = 100; // 10 kHz sample rate
	LPC_TIM0->TCR = 0;
	LPC_TIM0->TCR = 1;
	LPC_TIM0->PR = 0;
	LPC_TIM0->MR0 = SystemCoreClock / (8000000 / us);
	LPC_TIM0->MCR = 0b11;
	LPC_TIM0->EMR |= (0b11 << 6); // When match occurs, toggle MAT0.1
}
	
void setup() {
	init_lcd();
	LED_Initialize();
	LED_SetOut(0);
	
	init_adc();
	
	LED_SetOut(1);

	init_tim0();
	
	LED_SetOut(2);
}

void loop() {
	while(!(LPC_ADC->ADGDR >> 31)); // Wait for result
	
	uint16_t adc_val = (LPC_ADC->ADGDR >> 4) & 0xFFF;
	
	draw_buffer(buff, 0);
	for (uint32_t i = 0; i < SCOPE_MAX_X; i++) {
		buff[i] = adc_val * SCOPE_MAX_Y / 0xFFF;
	}
	draw_buffer(buff, 0b0000011111100000);
}

int main() {
	setup();
	while(1) loop();
}
