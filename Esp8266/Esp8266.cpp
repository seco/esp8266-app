#include "Esp8266.h"

Esp8266::Esp8266() :
  wiFiService(true),
  webService(true) {

  if (SPIFFS.begin()) {
    Log.verbose(F("File system mounted." CR));
  } else {
    Log.warning(F("Couldn't mount file system." CR));
  }
  Log.verbose(F("=========================" CR));
  Log.verbose(F("Setup finished. Have fun." CR));
  Log.verbose(F("=========================" CR));
}

void Esp8266::run() {

  if ((previousTime + updateInterval) < millis()) {
    previousTime = millis();

    Log.notice(F("do something else here ..." CR));
  }
}
