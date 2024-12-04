#include "LPC17xx.h"
#include "Board_LED.h"

void setup() {
	LED_Initialize();
}

void loop() {
	static int i = 0;
	LED_SetOut(i++ % 16);
	for(int j = 0; j < 1000000; j++) { }
}

int main() {
	setup();
	while(1) loop();
}
