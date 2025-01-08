#include "LCDControl.h"

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

void draw_traces(uint16_t* buff, uint16_t* old, uint32_t size, uint16_t color) {
	for (uint32_t i = 0; i < size; i++) {
		draw_point(i, old[i], 0);
		draw_point(i, buff[i], color);
	}
}

void draw_horizontal(uint16_t y, uint16_t x_max, uint16_t color) {
	for (uint16_t i = 0; i < x_max; i++) {
		draw_point(i, y, color);
	}
}
