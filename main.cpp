/*
 * main.cpp
 *
 *  Created on: 07.05.2015
 *      Author: gfu-mri
 */

#include "LCD.h"
#include "Control.h"
//#include <avr/interrupt.h>

#include <stdlib.h>

LCD lcd;
Control ctrl;

int main() {
	char tmp[5];
	int8_t lastBtn = 0;
	while(1) {
		if(ctrl.update()) {
			lcd.clear();
			lcd.print(itoa(ctrl.getValue()>>2, tmp, 10));
		}
		int8_t btn = ctrl.getButtons();
		if(lastBtn ^ btn) {
			lcd.setCursor(0, 1);
			lcd.print("Button");
			lastBtn = btn;
		}
	}
}
