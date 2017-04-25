#include "Esp8266.h"

Esp8266::Esp8266() {}

Esp8266::~Esp8266() {
  stop();
}

bool Esp8266::isRunning() {
  return _running;
}

bool Esp8266::start() {

  if (!isRunning()) {
    Log.verbose(F("Setup ESP8266 ..." CR));
  
    wiFiAPService.start();
    wiFiStaService.start();
    ntpService.start();
    mqttService.start();
    fsService.start();
    webService.start();

    espInfoHandler = &webService.on("/esp", HTTP_GET, getESPInfoFunction());
    infoHandler = &webService.on("/fs/info", HTTP_GET, fsService.getInfoFunction());
    listHandler = &webService.on("/fs/list", HTTP_GET, fsService.getListFunction());

    _running = true;

    Log.verbose(F("=========================" CR));
    Log.verbose(F("Setup finished. Have fun." CR));
    Log.verbose(F("=========================" CR));
  }
  
  return isRunning();
}

bool Esp8266::stop() {
  
  if (isRunning()) {
    webService.stop();
    fsService.stop();
    mqttService.stop();
    ntpService.stop();
    wiFiStaService.stop();
    wiFiAPService.stop();

    webService.remove(espInfoHandler);
    webService.remove(infoHandler);
    webService.remove(listHandler);
    
    _running = false;
  }

  return isRunning();
}

void Esp8266::run() {

  if ((previousTime + updateInterval) < millis()) {
    previousTime = millis();

    if (wiFiAPService.isRunning()) {
      Log.verbose(F("%d station(s) connected to soft-AP." CR), wiFiAPService.getStationNumber());
    }

    if (mqttService.isRunning()) {
      // generate some random example values
      float celsius = float(random(200, 215)) / 10;
      float humidity = float(random(100, 130)) / 10;

      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
      // add UTC timestamp to message
      if (ntpService.getNTPClient().update()) {
        json["timestamp"] = ntpService.getNTPClient().getEpochTime();
      } else {
        Log.error("Updating NTP failed.\n");
      }
      json["celsius"] = celsius;
      json["humidity"] = humidity;
      mqttService.publish(MQTT_PUB_TOPIC_SENSOR_A, json);
    }
  }
}

ArRequestHandlerFunction Esp8266::getESPInfoFunction() {
  
  return [](AsyncWebServerRequest *request) {
    // ESP8266 data from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h
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
  };
}
