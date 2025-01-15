#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdint.h>

#define SAMPLE_US 20 

typedef struct {
	uint16_t step;
	uint16_t vmax;
	uint16_t thresh;

	uint32_t dt;
	uint16_t dv;
} Control;

void EINT3_IRQHandler(void);
void init_control(void);
void init_uart(void);
uint16_t get_trig_y(void);

#endif
