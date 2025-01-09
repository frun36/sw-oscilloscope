#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdint.h>

typedef struct {
	uint16_t step;
	uint16_t vmax;
	uint16_t thresh;
} Control;

void EINT3_IRQHandler(void);
void init_control(void);

#endif
