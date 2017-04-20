#pragma once

#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <ESP8266mDNS.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/ESP8266mDNS.h

#ifdef ESP8266
extern "C" {
  #include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include "Service.h"

class WiFiAPService : public Service {

	public:

    WiFiAPService();

    ~WiFiAPService();

    bool isRunning();

    bool start();

    bool stop();

    bool setupMDNS();

    /**
     * returns number of connected stations
     */
    uint8_t getStationNumber();

  private:

    WiFiEventHandler connectedEventHandler, disconnectedEventHandler;
};
