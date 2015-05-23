/*
 * main.cpp
 *
 *  Created on: 07.05.2015
 *      Author: gfu-mri
 */

#include "LCD.h"
#include "Control.h"
#include <avr/interrupt.h>

#include <stdlib.h>

LCD lcd;
Control ctrl;

void initialize()
{

	//DDRB =	0b11111111;
	//DDRC =	0b11111111;
	//DDRD =	0b11111111;

	// Timer Counter 0 init - 8 bit
	// f = 7.812 kHz
	// Verwendung fuer Auswerung der Taster und analog Signal Erzeugung
	//TCCR0 = (0<<FOC0) | (1<<WGM00) | (0<<COM01) | (0<<COM00) | (1<<WGM01) | (0<<CS02) | (1<<CS01) | (0<<CS00);
	//TIMSK |= (1<<TOIE0); // Timer 0 OVF interrupt einschalten

	// Timer Counter 1 - Servo - 16 bit Counter - UINT16_MAX = 65535
	// Frequenz 50 Hz
	// Timer 1: f_PWM=f_Clk/(Prescaler*(1+ICR1))
	TCCR1A = (1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) |(0<<COM1B0) | (1<<WGM11) | (0<<WGM10);
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10); // Prescaler 256

/*	CS12	CS11	CS10	-> Prescaler
	0		0		1			   1
	0		1		0			   8
	0		1		1			  64
	1		0		0			 256
	1		0		1			1024	*/

	ICR1   = 1459 ;		// F�r welchen Wert im Register ICR1 ergibt sich eine Grundfrequenz von 50 Hz
	OCR1A = 0;			// Register zum Einstellen des Servo Tastgrades

	// Timer Counter 2 init - 8 bit
	// Verwendung fuer Motor-PWM
	// Timer 2: f = f_Clk / (256 * Prescaler2)
	// f2 = 7.812 kHz
	TCCR2 	= (0<<FOC2) | (1<<WGM20) | (1<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (1<<CS21) | (0<<CS20); // Prescaler2 = 8
	OCR2 	= 0;			// Register zum Einstellen des Motor Tastgrades
	TIMSK  |= (1<<TOIE2);   // Timer 2 OVF interrupt einschalten

	// Ergebnis steht in ADCH und ADCL
	//DDRA  &= (1<<PA0);  // ADC0 auf input setzen
	//DDRA  &= (1<<PA1);  // ADC1 auf input setzen
	//DDRA  &= (1<<PA2);	// ADC2 auf input setzen
}

int x;

//////////////////////////////////////////////////////////////////////////////////////
// Interupt Routine fuer Regelung
//////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER2_OVF_vect) {
	x++;

	ctrl.update();
}

//////////////////////////////////////////////////////////////////////////////////////
// Interupt Routine fuer Anzeige
//////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER0_OVF_vect)
{
}

int main() {

	char tmp[8];

	initialize();
	sei();
	while(1) {
		lcd.clear();

		lcd.print(itoa(ctrl.getValue()>>2, tmp, 10));
		lcd.setCursor(5, 0);
		lcd.print(itoa(x, tmp, 10));

		if(!(ctrl.getButtons() & (1<<SW_A))) {
			lcd.setCursor(0, 1);
			lcd.print("Button");
		}
		_delay_ms(50);
	}
}
