#include "Trigger.h"

uint8_t button_trigger() {
	return Buttons_GetState() & 1;
}

uint8_t level_trigger(uint16_t thresh, uint16_t val) {
	return val > thresh;
}

uint8_t edge_trigger(uint16_t thresh, uint16_t prev, uint16_t curr) {
	return prev < thresh && curr > thresh;
}
