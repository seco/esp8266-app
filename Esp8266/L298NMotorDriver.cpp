#include "L298NMotorDriver.h"

L298NMotorDriver::L298NMotorDriver(bool enabled, unsigned int pinPWM, unsigned int pinInputA, unsigned int pinInputB) : MotorDriver(enabled) {

	_pinPWM = pinPWM;
	_pinInputA = pinInputA;
	_pinInputB = pinInputB;
	_currentSpeed = 0;

	analogWriteRange(PMW_RANGE); // adjust PMW range
	pinMode(_pinPWM, OUTPUT);
	pinMode(_pinInputA, OUTPUT);
	pinMode(_pinInputB, OUTPUT);

  Log.verbose(F("Configuration for L298N done : pwm = %d, in1 = %d, in2 = %d" CR), pinPWM, pinInputA, pinInputB);
}

int L298NMotorDriver::getSpeed() const {
	return _currentSpeed;
}

void L298NMotorDriver::setSpeed(int speed) {
	// activate motors if the driver is enabled
	if (isEnabled()) {
		if (speed >= 0) {
      // limit speed if speed exceeds PMW_RANGE
      if (speed > PMW_RANGE) {
        speed = PMW_RANGE;
      }
			// if the speed is positive or 0 then move forward
			analogWrite(_pinPWM, speed);
			digitalWrite(_pinInputA, HIGH);
			digitalWrite(_pinInputB, LOW);
		} else {
			// limit speed if speed exceeds PMW_RANGE
      if (-speed > PMW_RANGE) {
        speed = -PMW_RANGE;
      }
			// if the speed is negative then move backward
			analogWrite(_pinPWM, -speed);
			digitalWrite(_pinInputA, LOW);
			digitalWrite(_pinInputB, HIGH);
		}
	}
  // save the current speed setting
  _currentSpeed = speed;
  Log.verbose(F("Current motor speed is %d" CR), getSpeed());
}

void L298NMotorDriver::apply(int speed) {
  setSpeed(_currentSpeed+speed);
}

