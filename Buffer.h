#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "LCDControl.h"
#include <cstdint>

#define BUFF_CAPACITY SCOPE_MAX_X

typedef struct {
	uint16_t old[BUFF_CAPACITY];
	uint16_t arr[BUFF_CAPACITY];
	uint32_t size;
} Buffer;

uint8_t buff_append(Buffer* buff, uint16_t val) {
	if (buff->size >= BUFF_CAPACITY)
		return 0;
	
	buff->arr[buff->size] = val;
	buff->size++;
	return 1;
}

void buff_clear(Buffer* buff) {
	for (uint32_t i = 0; i < buff->size; i++)
		buff->old[i] = buff->arr[i];
	buff->size = 0;
}

#endif
