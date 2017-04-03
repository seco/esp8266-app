#include "MotorShieldDriver.h"

MotorShieldDriver::MotorShieldDriver(bool enabled, unsigned int pinPWM, unsigned int pinDir) : MotorDriver(enabled) {

	_pinPWM = pinPWM;
	_pinDir = pinDir;
	_currentSpeed = 0;

	analogWriteRange(PMW_RANGE); // adjust PMW range
	pinMode(_pinPWM, OUTPUT);
	pinMode(_pinDir, OUTPUT);

  Log.verbose(F("Configuration for MotorShield done : pwd = %d, direction = %d" CR), pinPWM, pinDir);
}

int MotorShieldDriver::getSpeed() const {
	return _currentSpeed;
}

void MotorShieldDriver::setSpeed(int speed) {
	// activate motors if the driver is enabled
	if (isEnabled()) {
		if (speed >= 0) {
      // limit speed if speed exceeds PMW_RANGE
      if (speed > PMW_RANGE) {
        speed = PMW_RANGE;
      }
			// if the speed is positive or 0 then move forward
			analogWrite(_pinPWM, speed);
			digitalWrite(_pinDir, HIGH);
		} else {
			// limit speed if speed exceeds PMW_RANGE
      if (-speed > PMW_RANGE) {
        speed = -PMW_RANGE;
      }
			// if the speed is negative then move backward
			analogWrite(_pinPWM, -speed);
			digitalWrite(_pinDir, LOW);
		}
	}
  // save the current speed setting
  _currentSpeed = speed;
  Log.verbose(F("Current motor speed is %d" CR), getSpeed());
}

void MotorShieldDriver::apply(int speed) {
  setSpeed(_currentSpeed+speed);
}
