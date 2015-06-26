/*
 * Menu.cpp
 *
 *  Created on: 24.06.2015
 *      Author: phosfor
 */

#include "Menu.h"

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


/////////////////////////////////////////////

PidMenu::PidMenu(const char* name, PID<int>& pid)
	: SubMenu(name, pParams, 4),
	  params{
		{"kP", &pid.kP},
		{"kI", &pid.kI},
		{"kD", &pid.kD},
		{"Sum", &pid.sum, false}
	  },
	  pParams{&params[0], &params[1], &params[2], &params[3]}
	{
}
