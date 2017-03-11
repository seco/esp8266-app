#pragma once

#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <ESP8266WiFiMulti.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src/ESP8266WiFiMulti.h
#include <ESP8266mDNS.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/ESP8266mDNS.h

#include "Service.h"

class WiFiService : public Service {

	public:
		WiFiService(bool enabled, bool verbose);

    bool reset();

    bool setupAP(char const *ssid, char const *passwd);

    bool setupWiFi();

    bool setupMDNS();

  private:
    ESP8266WiFiMulti wifiMulti;
};
