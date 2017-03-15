#include "WiFiService.h"

WiFiService::WiFiService(bool enabled, bool verbose) : Service(enabled, verbose) {

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

  if (isVerbose()) {
     Serial.println(F("WiFi reset done."));
  }


  //WiFi.setOutputPower(wifiOutputPower);
  //WiFi.setPhyMode(WIFI_PHY_MODE_11B);
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);
  //WiFi.disconnect(true);


  return true; // TODO
}

bool WiFiService::setupAP(char const *ssid, char const *passwd) {

  bool succeeded = WiFi.softAP(ssid, passwd);
  delay(300);

  if (isVerbose()) {
    if (succeeded) {
      Serial.printf("IP address of AP is: %s\n", WiFi.softAPIP().toString().c_str());
    } else {
      Serial.println(F("Warning : Couldn't establish a soft access point."));
    }
  }

  return succeeded;
}

bool WiFiService::setupWiFi() {

  bool succeeded = false;

  // add access point credentials
  wifiMulti.addAP(WIFI_SSID_1, WIFI_PASSWD_1);
  wifiMulti.addAP(WIFI_SSID_2, WIFI_PASSWD_2);
  // try to connect to WiFi
  Serial.println(F("Trying to connect WiFi "));
  uint8_t i = 0;
  while (wifiMulti.run() != WL_CONNECTED && i++ < 20) { // retry 20 times = 10 seconds
    delay(300);
    Serial.print(F("."));
  }
  Serial.println(F("\n"));
  if (i > 20) {
    Serial.println(F("Warning : Couldn't connect to any WiFi. Please check your WiFi availability / accessibility and restart.\n"));
  } else {
    succeeded = true;
    Serial.printf("WiFi successful connected. IP address is: %s\n", WiFi.localIP().toString().c_str());
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

  if (isVerbose()) {
    if (succeeded) {
      Serial.printf("Open [http://%s.local] in your browser.\n", HOST_NAME);
    } else {
      Serial.printf("Warning : Couldn't set up [http://%s.local]\n", HOST_NAME);
    }
  }

  return succeeded;
}
