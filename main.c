#include "LPC17xx.h"
#include "Board_LED.h"
#include "LCDControl.h"
#include "Buffer.h"

Buffer buff;

void init_adc() {
	LPC_SC->PCONP |= (1 << 12); 			// Enable ADC
	LPC_ADC->ADCR = (1 << 1) 			| 	// ADC0.1
									(1 << 21) 		| 	// ADC Operational
									(5 << 8)  		| 	// Divide clock to 5 Mhz
									(0b100 << 24);  	// Start conversion on MAT0.1 rising edge (default, bit 27)
	
	LPC_PINCON->PINSEL1 |= (1 << 16); // P0.24 as ADC0.1
	
	LPC_ADC->ADINTEN |= (1 << 8); // Conversion on any channel generates interrupt
	NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_IRQHandler() {
	uint16_t adc_val = (LPC_ADC->ADGDR >> 4) & 0xFFF; // Clears global interrupt flag
	
	static uint32_t led = 0;
	
	if(!buff_append(&buff, adc_val * SCOPE_MAX_Y / 0xFFF)) {
		draw_buffer(buff.old, buff.size, 0x0000);
		draw_buffer(buff.arr, buff.size, 0x07E0);
		buff_clear(&buff);
	}

	LED_SetOut(led);
	led = (led ? 0 : 1);
}

void init_tim0() {
	// uint32_t us = 100; // 10 kHz sample rate
	uint32_t us = 1000;
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
	
	init_tim0();
	init_adc();
	
	LED_SetOut(1);
	
	LED_SetOut(2);
}

void loop() {
	__WFI();	
}

int main() {
	setup();
	while(1) loop();
}
