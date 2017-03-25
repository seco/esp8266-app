#include "L298NMotorDriver.h"

L298NMotorDriver::L298NMotorDriver(bool enabled, unsigned int pinPWM, unsigned int pinInput1, unsigned int pinInput2) : MotorDriver(enabled) {

	_pinPWM = pinPWM;
	_pinInput1 = pinInput1;
	_pinInput2 = pinInput2;
	_currentSpeed = 0;

	analogWriteRange(255); // change pwm range from 1023 to 255
	pinMode(_pinPWM, OUTPUT);
	pinMode(_pinInput1, OUTPUT);
	pinMode(_pinInput2, OUTPUT);

  Log.verbose(F("Configuration for L298N done : pwd = %d, in1 = %d, in2 = %d" CR), pinPWM, pinInput1, pinInput2);
}

int L298NMotorDriver::getSpeed() const {
	return _currentSpeed;
}

void L298NMotorDriver::setSpeed(int speed) {
	// save the current speed setting
	_currentSpeed = speed;
	// activate motors if the driver is enabled
	if (isEnabled()) {
		if (speed >= 0) {
			// if the speed is positive or 0 then move forward
			analogWrite(_pinPWM, speed);
			digitalWrite(_pinInput1, HIGH);
			digitalWrite(_pinInput2, LOW);
		} else {
			// if the speed is negative then move backward
			analogWrite(_pinPWM, -speed);
			digitalWrite(_pinInput1, LOW);
			digitalWrite(_pinInput2, HIGH);
		}
	}
 
  Log.verbose(F("Current motor speed is %d" CR), getSpeed());
}
