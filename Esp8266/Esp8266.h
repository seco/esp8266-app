#pragma once

#include <Arduino.h>
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <FS.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h

#include "Configuration.h"
#include "WiFiService.h"
#include "NTPService.h"
#include "MQTTService.h"
#include "WebService.h"

class Esp8266 {

	public:

		Esp8266();

		void run();

	private:

    WiFiService wiFiService;
		NTPService ntpService;
    MQTTService mqttService;
    WebService webService;

		unsigned long previousTime = millis();
		unsigned int updateInterval = 5000;
};
