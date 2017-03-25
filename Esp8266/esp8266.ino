#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log

#include "Esp8266.h"

Esp8266 *esp8266;

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(false);
  while(!Serial && !Serial.available()) {}
  
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  Log.verbose(F("Setup ESP8266 ..." CR));

  esp8266 = new Esp8266(); 
}

void loop() {
	esp8266->run();
}
