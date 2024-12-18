#ifndef _LCD_CONTROL_H_
#define _LCD_CONTROL_H_

#include "LCD/Open1768_LCD.h"
#include "LCD/LCD_ILI9325.h"

// Axes on the LCD are flipped
#define SCOPE_MAX_X LCD_MAX_Y
#define SCOPE_MAX_Y LCD_MAX_X

void fill(uint16_t color);
void init_lcd(void);
void draw_point(uint16_t x, uint16_t y, uint16_t color);
void draw_buffer(uint16_t* buff, uint32_t size, uint16_t color);

#endif
