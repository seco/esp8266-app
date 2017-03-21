#include "Esp8266.h"

Esp8266::Esp8266() :
  wiFiService(true, VERBOSE),
  webService(true, VERBOSE) {

  if (SPIFFS.begin()) {
    if (VERBOSE) {
      Serial.println(F("File system mounted."));
    }
  } else {
    if (VERBOSE) {
      Serial.println(F("Warning : Couldn't mount file system."));
    }
  }

  if (VERBOSE) {
    Serial.println(F("Setup done."));
  }
}

void Esp8266::run() {

  if ((previousTime + updateInterval) < millis()) {
    previousTime = millis();

    Serial.println(F("do something else ..."));
    
  }
}
