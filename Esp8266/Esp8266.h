#pragma once

#include <Arduino.h>

#include <FS.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h

#include "Configuration.h"
#include "L298NMotorDriver.h"
#include "WiFiService.h"
#include "WebService.h"

class Esp8266 {

	public:
		Esp8266();

		void run();

	private:

    L298NMotorDriver leftMotor;
    L298NMotorDriver rightMotor;

    WiFiService wiFiService;

    WebService webService;

		unsigned long previousTime = millis();
		unsigned int updateInterval = 5000;
};
