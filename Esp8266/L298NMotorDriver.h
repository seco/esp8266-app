#pragma once

#include "MotorDriver.h"

class L298NMotorDriver : public MotorDriver {

	public:
		L298NMotorDriver(bool enabled, unsigned int pinPWM, unsigned int pinInput1, unsigned int pinInput2);

		int getSpeed() const;

		void setSpeed(int speed);

	private:
		unsigned int _pinPWM;
		unsigned int _pinInput1;
		unsigned int _pinInput2;

		// keeps track of the current speed of the motor driver
		int _currentSpeed = 0;
};
