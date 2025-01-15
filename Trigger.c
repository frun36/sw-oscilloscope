#include "Trigger.h"
#include "Driver_USART.h"
#include <stdio.h>

extern ARM_DRIVER_USART* uart;

uint8_t level_trigger(uint16_t thresh, uint16_t val) {
	return val > thresh;
}

uint8_t edge_trigger(uint16_t thresh, uint16_t prev, uint16_t curr) {
#ifdef DEBUG
	char buff[32];
	uint32_t size;
	if (prev < thresh && curr > thresh) {
		size = sprintf(buff, "Trig:%u;%u;%u\r\n", prev, thresh, curr);
		uart->Send(buff, size);
		return 1;
	} else {
		return 0;
	} 
#endif

	return prev < thresh && curr > thresh;
}
