/*
 * ADC.h
 *
 *  Created on: 25.06.2015
 *      Author: phosfor
 */

#ifndef ADC_H_
#define ADC_H_


#include <stdint.h>

class Adc {
public:
	static volatile uint16_t AD0_links, AD1_rechts;
};

#endif /* ADC_H_ */
