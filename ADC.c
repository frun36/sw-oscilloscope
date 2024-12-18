#include "ADC.h"

void ADC_IRQHandler() {
	static uint16_t prev_val = 0;
	static uint8_t counter = 0;
	static uint8_t count_threshold = 10;
	static uint8_t triggered = 0;
	
	uint16_t adc_val = (LPC_ADC->ADGDR >> 4) & 0xFFF; // Clears global interrupt flag
	
	if (!triggered)
		triggered = edge_trigger(0xFFF / 2, prev_val, adc_val);
		// triggered = level_trigger(0xFFF / 2 + 0xFF, adc_val);
	
	if(!triggered)
		return;
	
	if(!buff_append(&buff, adc_val * SCOPE_MAX_Y / 0xFFF)) {
		LED_SetOut(1);
		draw_buffer(buff.old, buff.size, 0x0000);
		draw_buffer(buff.arr, buff.size, 0x07E0);
		buff_clear(&buff);
		triggered = 0;
		LED_SetOut(0);
	}
	
	if(counter == 0)
		prev_val = adc_val;
	
	counter = (counter + 1) % count_threshold;
}

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
