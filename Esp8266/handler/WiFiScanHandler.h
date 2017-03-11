#ifndef WIFI_SCAN_HANDLER_H
#define WIFI_SCAN_HANDLER_H

#include <ESPAsyncTCP.h> // https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

class WiFiScanHandler {
  public :
    WiFiScanHandler(){};

    void classRequest (AsyncWebServerRequest *request) {

      // WiFi scan isn't triggered = -2; https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFiScan.cpp
      if (WiFi.scanComplete() == -2) {
        WiFi.scanNetworks(false, true);
      }
      // WiFi scan not finished = -1;
      if (WiFi.scanComplete() == -1) {
        DEBUG_MSG("Scanning WiFi network ");
      }
      int i = 10;
      while (WiFi.scanComplete() == -1 && i > 0) {
        DEBUG_MSG(".");
        delay(100);
        i--;
      }

      int n = WiFi.scanComplete();
      if (n >= 0) {
        AsyncJsonResponse *response = new AsyncJsonResponse();
        JsonObject& json = response->getRoot();
        // write an entry for all found networks
        JsonArray& networks = json.createNestedArray("networks");
        for (int i = 0; i < n; ++i){
          JsonObject& entry = networks.createNestedObject();
          entry[F("rssi")] = WiFi.RSSI(i);
          entry[F("ssid")] = WiFi.SSID(i);
          entry[F("bssid")] = WiFi.BSSIDstr(i);
          entry[F("channel")] = WiFi.channel(i);
          entry[F("encryptionType")] = WiFi.encryptionType(i);
          entry[F("hidden")] = WiFi.isHidden(i)?"true":"false";
        }
        WiFi.scanDelete();
        response->setLength();
        request->send(response);
      } else {
        request->send(500, "Scanning WiFi networks failed");
      }



      /**
      int n = WiFi.scanComplete();
      if (n == -2){
        WiFi.scanNetworks(true);
      } else if(n) {
        for (int i = 0; i < n; ++i){
          JsonObject& entry = wiFis.createNestedObject();
          entry[F("rssi")] = WiFi.RSSI(i);
          entry[F("ssid")] = WiFi.SSID(i);
          entry[F("bssid")] = WiFi.BSSIDstr(i);
          entry[F("channel")] = WiFi.channel(i);
          entry[F("encryptionType")] = WiFi.encryptionType(i);
          entry[F("hidden")] = WiFi.isHidden(i)?"true":"false";
        }
        WiFi.scanDelete();
        if (WiFi.scanComplete() == -2){
          WiFi.scanNetworks(true);
        }
      }*/
    }
};

#endif
