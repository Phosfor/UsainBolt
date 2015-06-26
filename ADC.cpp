/*
 * ADC.cpp
 *
 *  Created on: 25.06.2015
 *      Author: phosfor
 */

#include "ADC.h"
#include <avr/interrupt.h>

volatile uint16_t Adc::AD0_links, Adc::AD1_rechts;

//////////////////////////////////////////////////////////////////////////////////////
// Interupt Routine fuer Einlesen Analogdigitalwandler
//////////////////////////////////////////////////////////////////////////////////////
ISR(ADC_vect)
{
	uint8_t analog_channel = ADMUX & (0b00011111);
	switch (analog_channel)
	{
	case 0:
		Adc::AD0_links = ADC;
		ADMUX |= (1<<MUX0); // auf Kanal 1 wechseln
	break;

	case 1:
		Adc::AD1_rechts = ADC;
		ADMUX &= ~(1<<MUX0);
		ADMUX &= ~(1<<MUX1); // auf Kanal 0 wechseln
	break;
	}
 	ADCSRA |= (1<<ADSC);  // start conversion
}

void Adc::init() {
	uint16_t result;


	ADMUX = (1<<REFS1) | (1<<REFS0);				// 2.6V als Referenz benutzen
	ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);	// Frequenzvorteiler
	ADCSRA |= (1<<ADEN);							// ADC aktivieren
	ADCSRA |= (1<<ADIE);							// ADC Interrupts erlauben

  	ADCSRA |= (1<<ADSC);                  			// eine ADC-Wandlung
  	while (ADCSRA & (1<<ADSC) ) {}        	        // auf Abschluss der Konvertierung warten

  	result = ADCW;
}
