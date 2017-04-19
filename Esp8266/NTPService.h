#pragma once

#include <WiFiUdp.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
#include <NTPClient.h> // https://github.com/arduino-libraries/NTPClient

#include "Service.h"

class NTPService : public Service {

	public:

		NTPService(bool enabled);

    NTPClient getNTPClient();

  private:

    WiFiUDP ntpUDP;
    NTPClient ntpClient;
};
