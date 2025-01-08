#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "Board_Joystick.h"
#include <stdint.h>

typedef struct {
	uint8_t v_range;
	uint8_t t_range;
} Control;

extern Control control;
void EINT3_IRQHandler(void);
void init_control(void);

#endif
