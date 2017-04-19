#include "NTPService.h"

NTPService::NTPService(bool enabled) :
  ntpClient(ntpUDP, NTP_SERVER, 3600, 60000),
  Service(enabled) {

  // start connection to NTP server
  ntpClient.begin();
}

NTPClient NTPService::getNTPClient() {

  if (!ntpClient.update()) {
    Log.error("Updating NTP failed.\n");
  }

  return ntpClient;
}
