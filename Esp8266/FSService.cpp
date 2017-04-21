#include "FSService.h"

FSService::FSService() {}

FSService::~FSService() {
  stop();
}

bool FSService::isRunning() {
  return _running;
}

bool FSService::start() {

  if (!isRunning()) {
    if (SPIFFS.begin()) {
      Log.verbose(F("File system mounted." CR));
      _running = true;
    } else {
      Log.warning(F("Mounting file system failed." CR));
    }
  }

  return isRunning();
}

bool FSService::stop() {

  if (isRunning()) {
    SPIFFS.end();
    _running = false;
  }

  return isRunning();
}
