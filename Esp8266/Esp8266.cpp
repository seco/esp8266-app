#include "Esp8266.h"

Esp8266::Esp8266() :
  wiFiService(true),
  ntpService(true),
  mqttService(true),
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

    // generate random values
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
