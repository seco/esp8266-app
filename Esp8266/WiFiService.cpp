#include "WiFiService.h"

WiFiService::WiFiService(bool enabled) : Service(enabled) {

	// reset former settings
  reset();

  // create access point (AP)
  #if (WIFI_MODE == WIFI_AP || WIFI_MODE == WIFI_AP_STA)
    setupAP(WIFI_AP_SSID, WIFI_AP_PASSWD);
  #endif

  // connect to WiFi
  #if (WIFI_MODE == WIFI_STA || WIFI_MODE == WIFI_AP_STA)
    setupWiFi();
  #endif

  // enable multicast DNS
  #ifdef HOST_NAME
    setupMDNS();
  #endif
}

bool WiFiService::reset() {

  // TODO have a deeper look at the WiFi reset procedure

  // disconnect from any previous WiFi connect
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  // set WiFi mode
  WiFi.mode(WIFI_MODE);
  WiFi.persistent(false);
  WiFi.hostname(HOST_NAME);
  // wait a moment until the new settings are applied
  delay(300);

  Log.verbose(F("WiFi reset done." CR));

  //WiFi.setOutputPower(wifiOutputPower);
  //WiFi.setPhyMode(WIFI_PHY_MODE_11B);
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);
  //WiFi.disconnect(true);


  return true; // TODO
}

bool WiFiService::setupAP(char const *ssid, char const *passwd) {

  bool succeeded = WiFi.softAP(ssid, passwd);
  delay(300);

  if (succeeded) {
    Log.notice(F("IP address of AP is: %s" CR), WiFi.softAPIP().toString().c_str());
  } else {
    Log.error(F("Couldn't establish a soft access point." CR));
  }

  return succeeded;
}

bool WiFiService::setupWiFi() {

  bool succeeded = false;

  // add access point credentials
  wifiMulti.addAP(WIFI_SSID_1, WIFI_PASSWD_1);
  wifiMulti.addAP(WIFI_SSID_2, WIFI_PASSWD_2);
  // try to connect to WiFi
  Log.verbose(F("Trying to connect WiFi "));
  uint8_t i = 0;
  while (wifiMulti.run() != WL_CONNECTED && i++ < 20) { // retry 20 times = 10 seconds
    delay(300);
    Serial.print(F("."));
  }
  Serial.println(F("\n"));
  if (i > 20) {
    Log.error(F("Couldn't connect to any WiFi. Please check your WiFi availability / accessibility and restart." CR));
  } else {
    succeeded = true;
    Log.notice(F("WiFi successful connected. IP address is: %s" CR), WiFi.localIP().toString().c_str());
  }

  return succeeded;
}

bool WiFiService::setupMDNS() {

  bool succeeded = false;
  // add <domain name>.local (mDNS)
  if (MDNS.begin(HOST_NAME)) {
    // add service
    MDNS.addService("http", "tcp", 80);
    //MDNS.addService("ws", "tcp", 81);
    succeeded = true;
  }

  if (succeeded) {
    Log.notice(F("Open [http://%s.local] in your browser." CR), HOST_NAME);
  } else {
    Log.warning(F("Couldn't set up [http://%s.local]" CR), HOST_NAME);
  }

  return succeeded;
}
