/*
 * Menu.h
 *
 *  Created on: 24.06.2015
 *      Author: phosfor
 */

#ifndef MENU_H_
#define MENU_H_

#include "PID.h"
#include "LCD.h"
#include "Control.h"

#include "util.h" //ftoa

#include <stdlib.h>
#include <stdint.h>

class Menu {
private:
	const char* name;
public:
	Menu(const char* name);

	virtual void display(LCD& lcd);
	virtual bool update(Control& ctrl) { return false; };

	const char* getName() const;
};

class SubMenu : public Menu {
private:
	Menu** children;
	const uint8_t count;
	int8_t select;
	bool inSubMenu;
public:
	SubMenu(const char* name, Menu** children, const uint8_t count);

	virtual void display(LCD& lcd) override;
	virtual bool update(Control& ctrl) override;

	bool isInSubMenu(const Menu* subMenu) const;
};

template<class T>
class NumberMenu : public Menu {
private:
	T* pValue;
	bool editable;
public:
	NumberMenu(const char* name, T* pValue, bool editable = true);

	virtual void display(LCD& lcd) override;
	virtual bool update(Control& ctrl) override;
};

class PidMenu : public SubMenu {
private:
	NumberMenu<int> params[4];
	Menu* pParams[4];
public:
	PidMenu(const char* name, PID<int>& pid);
};

/////////////////////////////////////////

template<class T>
NumberMenu<T>::NumberMenu(const char* name, T* pValue, bool editable) : Menu(name), pValue(pValue), editable(editable) {

}

template<class T>
void NumberMenu<T>::display(LCD& lcd) {
	Menu::display(lcd);
	lcd.setCursor(0, 1);

	char tmp[16];

	itoa(*pValue, tmp, 10);
	lcd.print(tmp);
}

template<class T>
bool NumberMenu<T>::update(Control& ctrl) {
	if(!editable) return false;
	int8_t delta = ctrl.getValue() >> 1;
	if(delta) {
		*pValue += delta;
		ctrl.resetValue();
		return true;
	}
	return false;
}

#endif /* MENU_H_ */
