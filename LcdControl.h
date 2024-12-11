#ifndef _LCD_CONTROL_H_
#define _LCD_CONTROL_H_

#include "LCD/Open1768_LCD.h"
#include "LCD/LCD_ILI9325.h"

// Axes on the LCD are flipped
#define SCOPE_MAX_X LCD_MAX_Y
#define SCOPE_MAX_Y LCD_MAX_X

void fill(uint16_t color) {
	lcdSetCursor(0, 0);
	lcdWriteIndex(DATA_RAM);
	for (uint32_t i = 0; i < LCD_MAX_X * LCD_MAX_Y; i++)
		lcdWriteData(color);
}

void init_lcd() {
	lcdConfiguration();
	init_ILI9325();
	lcdWriteReg(HADRPOS_RAM_START, 0);
	lcdWriteReg(HADRPOS_RAM_END, LCD_MAX_X);
	lcdWriteReg(VADRPOS_RAM_START, 0);
	lcdWriteReg(VADRPOS_RAM_END, LCD_MAX_Y);
	fill(LCDBlack);
}

void draw_point(uint16_t x, uint16_t y, uint16_t color) {
	lcdSetCursor(y, x);
	lcdWriteReg(DATA_RAM, color);
}

void draw_buffer(uint16_t* buff, uint32_t size, uint16_t color) {
	for (uint32_t i = 0; i < size; i++) {
		draw_point(i, buff[i], color);
	}
}

#endif
