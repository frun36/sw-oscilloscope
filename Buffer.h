#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "LCDControl.h"
#include <stdint.h>

#define BUFF_CAPACITY SCOPE_MAX_X

typedef struct {
	uint16_t old[BUFF_CAPACITY];
	uint16_t arr[BUFF_CAPACITY];
	uint32_t size;
} Buffer;

uint8_t buff_append(Buffer* buff, uint16_t val);
void buff_clear(Buffer* buff);

#endif
