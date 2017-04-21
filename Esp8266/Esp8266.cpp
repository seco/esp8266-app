#include "Esp8266.h"

Esp8266::Esp8266() {

  wiFiAPService.start();
  wiFiStaService.start();
  ntpService.start();
  mqttService.start();
  fsService.start();
  webService.start();

  webService.on("/info", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", "<span>ESP8266 TEST</span>");
  });

  //webServer.on("/esp", HTTP_GET, std::bind(&ESPHandler::request, espHandler, std::placeholders::_1));

  Log.verbose(F("=========================" CR));
  Log.verbose(F("Setup finished. Have fun." CR));
  Log.verbose(F("=========================" CR));
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
