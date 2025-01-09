#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdint.h>

#define SAMPLE_US 10 

typedef struct {
	uint16_t step;
	uint16_t vmax;
	uint16_t thresh;

	uint16_t dt;
	uint16_t dv;
} Control;

void EINT3_IRQHandler(void);
void init_control(void);

#endif
