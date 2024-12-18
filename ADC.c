#include "ADC.h"

extern Buffer buff;
extern uint8_t do_draw;

uint32_t get_mv(uint32_t adc_val) {
	uint32_t diff = ADC_VMAX - ADC_VMIN;
	
	return diff * adc_val / ADC_SMAX - ADC_VMIN;
}

void ADC_IRQHandler() {
	do_draw = 0;
	static uint16_t prev_val = 0;
	static uint8_t counter = 0;
	static uint8_t count_threshold = 8;
	static uint8_t triggered = 0;
	
	uint16_t adc_val = (LPC_ADC->ADGDR >> 4) & 0xFFF; // Clears global interrupt flag
	
	if (!triggered)
		triggered = edge_trigger(0xFFF / 2, prev_val, adc_val);
	
	if(!triggered)
		return;
	
	if(!buff_append(&buff, adc_val * SCOPE_MAX_Y / 0xFFF)) {
		triggered = 0;
		do_draw = 1;
	}
	
	if(counter == 0)
		prev_val = adc_val;
	
	counter = (counter + 1) % count_threshold;
}

void handle_draw() {
	if (!do_draw)
		return;
	LED_On(3);
	draw_buffer(buff.old, buff.size, 0x0000);
	draw_buffer(buff.arr, buff.size, 0x07E0);
	buff_clear(&buff);
	LED_Off(3);
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
	NVIC_SetPriority(ADC_IRQn, 1);
}
