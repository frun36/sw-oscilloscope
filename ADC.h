#ifndef _ADC_H_
#define _ADC_H_

#include "LPC17xx.h"
#include "Buffer.h"
#include "Trigger.h"
#include "Board_LED.h"

// in mV
#define ADC_VMIN 0
#define ADC_VMAX 3300

// samples
#define ADC_SMAX 0xFFF

// Requires a buffer to be present in main

void ADC_IRQHandler(void);

void init_tim0();
void init_adc(void);
void handle_draw(void);

#endif
