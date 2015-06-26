/*
 * main.cpp
 *
 *  Created on: 07.05.2015
 *      Author: gfu-mri
 */

#include "LCD.h"
#include "Control.h"
#include "Menu.h"
#include "ADC.h"

#include <avr/interrupt.h>

#include <stdlib.h>

#define MAX_LENKUNG 800

LCD lcd;
Control ctrl;

int speed, sollSpeed;

int lenkung;
NumberMenu<int> lenkungMenu("Lenkung", &lenkung);

int power = 25;
NumberMenu<int> powerMenu("Power", &power);

uint16_t minAdc = 7;
NumberMenu<uint16_t> minAdcMenu("MinAdc", &minAdc);

PID<int> lenkungPid(35, 1, 3);
PidMenu lenkungPidMenu("Lenkung", lenkungPid);

PID<int> powerPid(0, 0, 0);
PidMenu powerPidMenu("Power", powerPid);

Menu* mcsub[] = {
	&lenkungMenu,
	&powerMenu
};
SubMenu manualMenu("Manuell", mcsub, 2);

Menu* pidsub[] = {
	&lenkungPidMenu,
	&powerPidMenu
};
SubMenu pidMenu("PID", pidsub, 2);

NumberMenu<volatile uint16_t> ad0Menu("AD0 Links", &Adc::AD0_links, false);
NumberMenu<volatile uint16_t> ad1Menu("AD1 Rechts", &Adc::AD1_rechts, false);

Menu* viewsub[] = {
		&ad0Menu,
		&ad1Menu
};
SubMenu viewMenu("View", viewsub, 2);

Menu* mmsub[] = {
	&manualMenu,
	&viewMenu,
	&pidMenu,
	&minAdcMenu
};
SubMenu mainMenu("Main", mmsub, 4);

void initialize()
{

	//DDRB =	0b11111111;
	//DDRC =	0b11111111;
	DDRD =	0b11111111;

	// Timer Counter 0 init - 8 bit
	// f = 7.812 kHz
	// Verwendung fuer Auswerung der Taster und analog Signal Erzeugung
	//TCCR0 = (0<<FOC0) | (1<<WGM00) | (0<<COM01) | (0<<COM00) | (1<<WGM01) | (0<<CS02) | (1<<CS01) | (0<<CS00);
	//TIMSK |= (1<<TOIE0); // Timer 0 OVF interrupt einschalten

	// Timer Counter 1 - Servo - 16 bit Counter - UINT16_MAX = 65535
	// Frequenz 50 Hz
	// Timer 1: f_PWM=f_Clk/(Prescaler*(1+ICR1))
	TCCR1A = (1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) |(0<<COM1B0) | (1<<WGM11) | (0<<WGM10);
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10); // Prescaler 8

/*	CS12	CS11	CS10	-> Prescaler
	0		0		1			   1
	0		1		0			   8
	0		1		1			  64
	1		0		0			 256
	1		0		1			1024	*/

	ICR1   = 39999 ;		// F�r welchen Wert im Register ICR1 ergibt sich eine Grundfrequenz von 50 Hz
	OCR1A = 3000;			// Register zum Einstellen des Servo Tastgrades

	// Timer Counter 2 init - 8 bit
	// Verwendung fuer Motor-PWM
	// Timer 2: f = f_Clk / (256 * Prescaler2)
	// f2 = 7.812 kHz
	TCCR2 	= (0<<FOC2) | (1<<WGM20) | (1<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (1<<CS21) | (0<<CS20); // Prescaler2 = 8
	OCR2 	= 0;			// Register zum Einstellen des Motor Tastgrades
	TIMSK  |= (1<<TOIE2);   // Timer 2 OVF interrupt einschalten

	// Ergebnis steht in ADCH und ADCL
	DDRA  &= (1<<PA0);  // ADC0 auf input setzen
	DDRA  &= (1<<PA1);  // ADC1 auf input setzen
	//DDRA  &= (1<<PA2);	// ADC2 auf input setzen


}

//////////////////////////////////////////////////////////////////////////////////////
// Interupt Routine fuer Regelung
//////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER2_OVF_vect) {
	ctrl.update();

	uint16_t adcSum = Adc::AD0_links + Adc::AD1_rechts;
	if(adcSum > minAdc) {
		if(!(PORTD & (1<<PD6)))
			lenkungPid.resetSum();
		PORTD |= (1<<PD6);
	}
	else
		PORTD &= ~(1<<PD6);

	int16_t adcDiff = Adc::AD0_links - Adc::AD1_rechts;

	if(!mainMenu.isInSubMenu(&manualMenu)) {
		lenkung = lenkungPid.update(0, adcDiff);
		//power = powerPid.update(sollSpeed, 0);
	}

	if(lenkung < -MAX_LENKUNG) lenkung = -1000;
	else if(lenkung > MAX_LENKUNG) lenkung = 1000;

	OCR1A = 2200 + lenkung;



	if(power > 200) power = 200;
	else if(power < 0) power = 0;

	OCR2 = power;
}

//////////////////////////////////////////////////////////////////////////////////////
// Interupt Routine fuer Anzeige
//////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER0_OVF_vect)
{
}

int main() {

	initialize();
	Adc::init();
	sei();
	ADCSRA |= (1<<ADSC);  // ADC starten
	PORTD |= (1<<PD6);

	mainMenu.display(lcd);

	while(1) {
		mainMenu.update(ctrl);
		mainMenu.display(lcd);

		_delay_ms(50);
	}
}
