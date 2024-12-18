#ifndef _TRIGGER_H_
#define _TRIGGER_H_

#include <stdint.h>

uint8_t level_trigger(uint16_t thresh, uint16_t val);
uint8_t edge_trigger(uint16_t thresh, uint16_t prev, uint16_t curr);

#endif
