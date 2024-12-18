#ifndef _ADC_H_
#define _ADC_H_

#include "LPC17xx.h"
#include "Buffer.h"
#include "Trigger.h"
#include "Board_LED.h"

// in mV
#define ADC_VMIN 0
#define ADC_VMAX 3300

extern Buffer buff;

void ADC_IRQHandler(void);

void init_adc(void);

#endif
