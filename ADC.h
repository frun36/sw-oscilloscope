#ifndef _ADC_H_
#define _ADC_H_

// in mV
#define ADC_VMIN 0
#define ADC_VMAX 3300

// samples
#define ADC_SMAX 0xFFF

void ADC_IRQHandler(void);

void init_tim0(void);
void init_adc(void);
void handle_draw(void);

#endif
