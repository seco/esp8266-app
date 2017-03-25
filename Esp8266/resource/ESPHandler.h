#pragma once

#ifdef ESP8266
extern "C" {
  #include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

class ESPHandler {
  public:
    ESPHandler() {};

    void classRequest(AsyncWebServerRequest *request) {

      // return data from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h
      AsyncJsonResponse *response = new AsyncJsonResponse();
      JsonObject& json = response->getRoot();
      json[F("vcc")] = ESP.getVcc();
      json[F("heap")] = ESP.getFreeHeap();
      json[F("chipId")] = ESP.getChipId();
      json[F("sdkVersion")] = ESP.getSdkVersion();
      json[F("coreVersion")] =  ESP.getCoreVersion();
      json[F("bootVersion")] = ESP.getBootVersion();
      json[F("bootMode")] = ESP.getBootMode();
      json[F("cpuFreqMHz")] = ESP.getCpuFreqMHz();
      json[F("flashChipId")] = ESP.getFlashChipId();
      json[F("flashChipRealSize")] = ESP.getFlashChipRealSize();
      json[F("flashChipSize")] = ESP.getFlashChipSize();
      json[F("flashChipSpeed")] = ESP.getFlashChipSpeed();
      json[F("flashChipMode")] = ESP.getFlashChipMode();
      json[F("flashChipSizeByChipId")] = ESP.getFlashChipSizeByChipId();
      json[F("sketchSize")] = ESP.getSketchSize();
      json[F("sketchMD5")] = ESP.getSketchMD5();
      json[F("freeSketchSpace")] = ESP.getFreeSketchSpace();
      json[F("resetReason")] = ESP.getResetReason();
      json[F("resetInfo")] = ESP.getResetInfo();
      json[F("cycleCount")] = ESP.getCycleCount();
      json[F("uptime")] = millis();

      //json[F("analog")] = analogRead(A0);
      //json[F("gpio")] = String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));

      response->setLength();
      request->send(response);
    }
};
