#ifndef _LCD_CONTROL_H_
#define _LCD_CONTROL_H_

#include "LCD/Open1768_LCD.h"
#include "LCD/LCD_ILI9325.h"

#define GRID_COLOR 0xEEEE
#define TRIG_COLOR 0xDD00

// Axes on the LCD are flipped
#define SCOPE_MAX_X LCD_MAX_Y
#define SCOPE_MAX_Y (LCD_MAX_X - 20)

void fill(uint16_t color);
void init_lcd(void);
void draw_point(uint16_t x, uint16_t y, uint16_t color);
void draw_traces(uint16_t* buff, uint16_t* old, uint32_t size, uint16_t color);
void draw_horizontal(uint16_t y, uint16_t color);
void draw_vertical(uint16_t x, uint16_t color);

void draw_char(uint16_t x0, uint16_t y0, unsigned char c, uint16_t fg_color, uint16_t bg_color);
void draw_string(uint16_t x0, uint16_t y0, const char* str, uint16_t n, uint16_t fg_color, uint16_t bg_color);

#endif
