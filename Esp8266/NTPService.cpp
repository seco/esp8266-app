#include "NTPService.h"

NTPService::NTPService() : 
  ntpClient(ntpUDP, NTP_SERVER, 3600, 60000) {}

NTPService::~NTPService() {
  stop();
}

bool NTPService::isRunning() {
  return _running;
}

bool NTPService::start() {
  
  if (!isRunning()) {
    ntpClient.begin();
    _running = ntpClient.forceUpdate();
    if (_running) {
      Log.verbose(F("NTP established to [%s:%d]" CR), NTP_SERVER, NTP_DEFAULT_LOCAL_PORT);
    } else {
      Log.error(F("NTP failed for [%s:%d]" CR), NTP_SERVER, NTP_DEFAULT_LOCAL_PORT);
    }
  }

  return isRunning();
}

bool NTPService::stop() {
  
  if (isRunning()) {
    ntpClient.end();
    _running = false;
  }

  return isRunning();
}

NTPClient NTPService::getNTPClient() {

  if (!ntpClient.update()) {
    Log.error("Updating NTP failed.\n");

    // TODO implement retry and/or recovery mechanisms
  }

  return ntpClient;
}
