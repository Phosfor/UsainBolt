/*
 * Menu.h
 *
 *  Created on: 24.06.2015
 *      Author: phosfor
 */

#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>
#include "PID.h"

class LCD;
class Control;

class Menu {
private:
	const char* name;
public:
	Menu(const char* name);

	virtual void display(LCD& lcd);
	virtual bool update(Control& ctrl);

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

class FloatMenu : public Menu {
private:
	float* pValue;
	const float factor;
public:
	FloatMenu(const char* name, float* pValue, const float factor);

	virtual void display(LCD& lcd) override;
	virtual bool update(Control& ctrl) override;
};

class PidMenu : public SubMenu {
private:
	FloatMenu params[3];
	Menu* pParams[3];
public:
	PidMenu(const char* name, PID<float>& pid);
};

#endif /* MENU_H_ */
