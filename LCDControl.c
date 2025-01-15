#include "LCDControl.h"
#include "LCD/asciiLib.h"

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
	draw_horizontal(0, GRID_COLOR);
	draw_horizontal(SCOPE_MAX_Y / 2, GRID_COLOR);
	draw_horizontal(SCOPE_MAX_Y, GRID_COLOR);
	
	draw_vertical(0, GRID_COLOR);
	draw_vertical(SCOPE_MAX_X / 2, GRID_COLOR);
	draw_vertical(SCOPE_MAX_X, GRID_COLOR);
}

static uint8_t is_on_grid(uint16_t x, uint16_t y) {
	return x == 0 || x == SCOPE_MAX_X / 2 || x == SCOPE_MAX_X || y == 0 || y == SCOPE_MAX_Y / 2 || y == SCOPE_MAX_Y;
}

void draw_point(uint16_t x, uint16_t y, uint16_t color) {
	lcdSetCursor(y, x);
	lcdWriteReg(DATA_RAM, color);
}

void draw_traces(uint16_t* buff, uint16_t* old, uint32_t size, uint16_t color) {
	for (uint32_t i = 0; i < size; i++) {
		draw_point(i, old[i], is_on_grid(i, old[i]) ? GRID_COLOR : 0);
		draw_point(i, buff[i], color);
	}
}

void draw_horizontal(uint16_t y, uint16_t color) {
	if (color == 0 && is_on_grid(1, y))
		color = GRID_COLOR;

	for (uint16_t i = 0; i < SCOPE_MAX_X; i++) {
		draw_point(i, y, color);
	}
}

void draw_vertical(uint16_t x, uint16_t color) {
	if (color == 0 && is_on_grid(x, 1))
		color = GRID_COLOR;

	for (uint16_t j = 0; j < SCOPE_MAX_Y; j++) {
		draw_point(x, j, color);
	}
}

void draw_char(uint16_t x0, uint16_t y0, unsigned char c, uint16_t fg_color, uint16_t bg_color) {
	unsigned char buff[16];
	GetASCIICode(0, buff, c);

	uint16_t color;
	uint16_t x;
	uint16_t y;
	for (uint16_t i = 0; i < 16; i++) {
		for (uint16_t j = 0; j < 8; j++) {
			color = (buff[i] & (1 << (7 - j))) ? fg_color : bg_color;
			x = x0 + j;
			y = y0 - i;
			draw_point(x, y, color);
		}
	}
}

void draw_string(uint16_t x0, uint16_t y0, const char* str, uint16_t n, uint16_t fg_color, uint16_t bg_color) {
	for (uint16_t i = 0; i < n; i++) {
		draw_char(x0 + i * 8, y0, str[i], fg_color, bg_color);
	}
}
