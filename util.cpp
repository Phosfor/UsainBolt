/*
 * util.cpp
 *
 *  Created on: 24.06.2015
 *      Author: phosfor
 */

#include "util.h"
#include <stdlib.h>
#include <string.h>


int ipow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

/*char* ftoa(float value, char* buf, uint8_t precision) {
	char* res = buf;
	int x = (int) value;
	itoa(x, buf, 10);
	buf += strlen(buf);
	*buf++ = '.';


	value -= x;
	if(x < 0) {
		value *= -1;
	}

	while(precision) {
		value *= 10;

		x = (int) value;
		*buf++ += '0' + (char)x;

		precision--;
		value -= x;
	}
	*buf = '\0';

	return res;
}*/

char *ftoa(char *buffer, double d, int precision) {

	char *endOfString = buffer;
	//(workaround) -1 < d < 0 -> wholePart is 0 but it's negative -> itoa does not put the '-'
	if(d < 0 && d > -1)
		*endOfString++ = '-';

	long wholePart = (long) d;

	// Deposit the whole part of the number.

	itoa(wholePart,endOfString,10);

	// Now work on the faction if we need one.

	if (precision > 0) {

		// We do, so locate the end of the string and insert
		// a decimal point.

		while (*endOfString != '\0') endOfString++;
		*endOfString++ = '.';

		// Now work on the fraction, be sure to turn any negative
		// values positive.

		if (d < 0) {
			d *= -1;
			wholePart *= -1;
		}

		double fraction = d - wholePart;
		while (precision > 0) {

			// Multipleby ten and pull out the digit.

			fraction *= 10;
			wholePart = (long) fraction;
			*endOfString++ = '0' + wholePart;

			// Update the fraction and move on to the
			// next digit.

			fraction -= wholePart;
			precision--;
		}

		// Terminate the string.

		*endOfString = '\0';
	}

   return buffer;
}
