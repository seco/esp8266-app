#pragma once

#include "Driver.h"

class MotorDriver : public Driver {

	public:

		MotorDriver(bool enabled, bool verbose) : Driver(enabled, verbose) {
		}

		// returns current motor speed
		virtual int getSpeed() const = 0;

		// set motor speed
		// speed value = 0 and the motor stops
		// speed value between 1 and 255 and the motor runs forward
		// speed value between -255 and -1 and the motor runs backwards
		virtual void setSpeed(int speed) = 0;
};
