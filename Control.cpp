/*
 * Control.cpp
 *
 *  Created on: 21.05.2015
 *      Author: phosfor
 */

#include "Control.h"

Control::Control() {
	//DDR auf input setzen
	DGB_DDR &= ~(1<<DGB_A);
	DGB_DDR &= ~(1<<DGB_B);
	DGB_DDR &= ~(1<<DGB_SW);

	//PULL UP Einschalten
	DGB_PORT |= (1<<DGB_A);
	DGB_PORT |= (1<<DGB_B);
	DGB_PORT |= (1<<DGB_SW);

	//Initialisierung der zusaetzlichen Schalter
	//DDR auf input setzen
//	SW_DDR &= ~(1<<SW_1);
	SW_DDR &= ~(1<<SW_A);
	SW_DDR &= ~(1<<SW_B);
	//PULL UP Einschalten
//	SW_PORT |= (1<<SW_1);
	SW_PORT |= (1<<SW_A);
	SW_PORT |= (1<<SW_B);

	update();
	value = 0;
}

bool Control::update() {
	updateDeltaButtons = true;

	int8_t current = 0;
	if(DGB_PIN & (1<<DGB_A))
		current = 3;
	if(DGB_PIN & (1<<DGB_B))
		current ^= 1;
	int8_t diff = last - current;
	if(diff & 1) {
		last = current;
		value += (diff & 2) - 1;
		return true;
	}
	return false;
}

int8_t Control::getValue() const {
	return value;
}

uint8_t Control::getButtons() const {
	return ~(SW_PIN & ((1<<SW_A) | (1<<SW_B)));// | (SW_PIN & (1<<SW_1))
}

void Control::resetValue() {
	value = 0;
}

uint8_t Control::getDeltaButtons() {
	static uint8_t res = 0;
	static uint8_t last = getButtons();
	if(updateDeltaButtons) {
		updateDeltaButtons = false;
		uint8_t current = getButtons();
		res = last & ~current;
		last = current;
	}
	return res;
}
