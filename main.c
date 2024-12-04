#include "LPC17xx.h"
#include "Board_LED.h"
#include "LCD/LCD_ILI9325.h"
#include "LCD/Open1768_LCD.h"

typedef uint16_t Color;

void fill(Color color) {
	lcdSetCursor(0, 0);
	lcdWriteIndex(DATA_RAM);
	for (uint32_t i = 0; i < LCD_MAX_X * LCD_MAX_Y; i++)
		lcdWriteData(color);
}

void setup() {
	lcdConfiguration();
	init_ILI9325();
	lcdWriteReg(HADRPOS_RAM_START, 0);
	lcdWriteReg(HADRPOS_RAM_END, LCD_MAX_X);
	lcdWriteReg(VADRPOS_RAM_START, 0);
	lcdWriteReg(VADRPOS_RAM_END, LCD_MAX_Y);
	LED_Initialize();
	LED_SetOut(0);
	fill(LCDBlack);
}

void loop() {
	static Color i = 0;
	LED_SetOut(i++ % 16);
	fill(i);
	for(int j = 0; j < 1000000; j++) {	}
}

int main() {
	setup();
	while(1) loop();
}
