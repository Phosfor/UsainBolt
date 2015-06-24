/*
 * Control.h
 *
 *  Created on: 21.05.2015
 *      Author: phosfor
 */

#ifndef CONTROL_H_
#define CONTROL_H_


#include <avr/io.h>
#include <stdint.h>


#define DGB_PORT	PORTB
#define DGB_DDR		DDRB
#define DGB_A		PB0
#define DGB_B		PB1
#define DGB_SW		PB2
#define DGB_PIN		PINB

// Definitionen für zusaetzliche Schalter
#define SW_PORT		PORTB
#define SW_DDR		DDRB
#define SW_PIN		PINB
//#define SW_1		PC6  //
#define SW_A		PB3  // Schalter A
#define SW_B		PB4  // Schalter B

class Control {
private:
	int8_t value;
	int8_t last;

	bool updateDeltaButtons;
public:
	Control();

	bool update(); //Should be called pretty often ;D

	int8_t getValue() const;
	uint8_t getButtons() const;

	uint8_t getDeltaButtons();

	void resetValue();
};

#endif /* CONTROL_H_ */
