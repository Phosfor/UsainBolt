/*
 * PID.h
 *
 *  Created on: 24.05.2015
 *      Author: phosfor
 */

#ifndef PID_H_
#define PID_H_

#ifdef PID_CHECKMAX
	#define MAX_INT(T) (((unsigned T)-1)>>1)
#else
	#define ADD(a, b) (a+b)
#endif

template<class T>
class PID {
private:
	T kP, kI, kD;
	T lastValue;
	T sum;
public:
	PID(T kP, T kI, T kD) : kP(kP), kI(kI), kD(kD) {}
	T update(T setpoint, T value) {
		T error = setpoint - value;

		sum = ADD(sum, error);

		T diff = ADD(error, -lastValue);
		lastValue = error;

		return kP*error + kI*sum + kD*diff;
	}
#ifdef PID_CHECKMAX
	static T ADD(T value, T error) {
		if(error > 0 && MAX_INT(T) - error < value) // <=> value + error > MAX_INT
			return MAX_INT(T);
		else if(error < 0 && MAX_INT(T) + value) // <=> value + error < MIN_INT
			return -MAX_INT(T);
		else
			return value + error;
	}
#endif
};

#endif /* PID_H_ */
