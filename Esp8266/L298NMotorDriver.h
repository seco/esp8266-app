#pragma once

#include "MotorDriver.h"

class L298NMotorDriver : public MotorDriver {

	public:

		L298NMotorDriver(bool enabled, unsigned int pinPWM, unsigned int pinInputA, unsigned int pinInputB);

		int getSpeed() const;

		void setSpeed(int speed);

	private:

		unsigned int _pinPWM;
		unsigned int _pinInputA;
		unsigned int _pinInputB;

		// keeps track of the current speed of the motor driver
		int _currentSpeed = 0;
};
