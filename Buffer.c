#include "Buffer.h"

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