#include "ADC.h"
#include "Control.h"

extern Buffer buff;
uint8_t do_draw = 0;

extern Control control;

uint16_t get_mv(uint32_t adc_val) {
	static uint32_t diff = ADC_VMAX - ADC_VMIN;
	
	return diff * adc_val / ADC_SMAX - ADC_VMIN;
}

void ADC_IRQHandler() {
	static uint16_t prev_mv = 0;
	static uint32_t counter = 0;
	static uint8_t count_threshold = 8;
	static uint8_t triggered = 0;
	
	LED_On(2);
	do_draw = 0;	
	counter++;
	
	uint16_t adc_val = (LPC_ADC->ADGDR >> 4) & 0xFFF; // Clears global interrupt flag
	uint16_t mv = get_mv(adc_val);
	
	if(counter % count_threshold == 0)
		prev_mv = mv;
	
	if (!triggered)
		triggered = edge_trigger(control.thresh, prev_mv, mv);
	
	if(!triggered)
		return;

  if(counter % control.step != 0)
    return;
	
	uint16_t scope_value = mv > control.vmax ? SCOPE_MAX_Y : mv * SCOPE_MAX_Y / control.vmax;
	if(!buff_append(&buff, scope_value)) {
		triggered = 0;
		do_draw = 1;
	}
}

void handle_draw() {
	if (!do_draw)
		return;
    
	LED_On(3);
	draw_traces(buff.arr, buff.old, buff.size, 0x07E0);
	buff_clear(&buff);
	
	LED_Off(3);
}

void init_tim0() {
	LPC_TIM0->TCR = 0;
	LPC_TIM0->TCR = 1;
	LPC_TIM0->PR = 0;
	LPC_TIM0->MR0 = SystemCoreClock / (8000000 / SAMPLE_US);
	LPC_TIM0->MCR = 0b11;
	LPC_TIM0->EMR |= (0b11 << 6); // When match occurs, toggle MAT0.1
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
