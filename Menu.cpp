/*
 * Menu.cpp
 *
 *  Created on: 24.06.2015
 *      Author: phosfor
 */

#include "Menu.h"
#include "LCD.h"
#include "Control.h"

#include "util.h"

#include <stdlib.h> //ftoa

Menu::Menu(const char* name) : name(name) {
}
void Menu::display(LCD& lcd) {
	lcd.clear();
	lcd.print(name);
}

const char* Menu::getName() const {
	return name;
}


///////////////////////////////////////

SubMenu::SubMenu(const char* name, Menu** children, const uint8_t count) : Menu(name), children(children), count(count), select(0), inSubMenu(false) {

}

void SubMenu::display(LCD& lcd) {
	if(inSubMenu) children[select]->display(lcd);
	else {
		Menu::display(lcd);
		lcd.setCursor(0, 1);
		lcd.print(children[select]->getName());
	}
}

bool SubMenu::update(Control& ctrl) {
	if(inSubMenu) {
		if(children[select]->update(ctrl))
			return true;
		if(ctrl.getDeltaButtons() & (1<<SW_B)) {
			inSubMenu = false;
			return true;
		}
	} else {
		int8_t delta = ctrl.getValue() >> 2;
		if(delta) {
			select += delta;
			if(select >= count) select = count-1;
			if(select < 0) select = 0;
			ctrl.resetValue();
			return true;
		}

		if(ctrl.getDeltaButtons() & (1<<SW_A)) {
			inSubMenu = true;
			return true;
		}
	}
	return false;
}

bool SubMenu::isInSubMenu(const Menu* menu) const {
	if(!inSubMenu) return false;
	if(!menu) return true;
	return children[select] == menu;
}

/////////////////////////////////////////

FloatMenu::FloatMenu(const char* name, float* pValue, const float factor) : Menu(name), pValue(pValue), factor(factor) {

}

void FloatMenu::display(LCD& lcd) {
	Menu::display(lcd);
	lcd.setCursor(0, 1);

	char tmp[16];

	ftoa(tmp, *pValue, 5);
	lcd.print(tmp);
}

bool FloatMenu::update(Control& ctrl) {
	int8_t delta = ctrl.getValue() >> 1;
	if(delta) {
		*pValue += factor * delta;
		ctrl.resetValue();
		return true;
	}
	return false;
}

/////////////////////////////////////////////

PidMenu::PidMenu(const char* name, PID<float>& pid)
	: SubMenu(name, pParams, 3),
	  params{
		{"kP", &pid.kP, 0.1},
		{"kI", &pid.kI, 0.1},
		{"kD", &pid.kD, 0.1}
	  },
	  pParams{&params[0], &params[1], &params[2]}
	{
}
