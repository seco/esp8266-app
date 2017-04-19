#include "MQTTService.h"

MQTTService::MQTTService() {}

MQTTService::~MQTTService() {
  stop();
}

bool MQTTService::isRunning() {
  return mqttClient.connected();
}

bool MQTTService::start() {

  if (!isRunning()) {
    // add mqtt callback handlers : https://github.com/marvinroger/async-mqtt-client/blob/master/docs/2.-API-reference.md
    mqttClient.onConnect(onConnect);
    mqttClient.onDisconnect(onDisconnect);
    mqttClient.onPublish(onPublish);

    // configure mqtt client
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setCredentials(MQTT_USERNAME, MQTT_PASSWD);
    mqttClient.setClientId(MQTT_CLIENT);
    mqttClient.setKeepAlive(15); // in seconds
    mqttClient.setCleanSession(true);
    // try to connect
    mqttClient.connect();
    // wait a moment to establish a connection
    delay(300);
    if (isRunning()) {
      Log.verbose(F("MQTT connection successful established to [%s:%d]" CR), MQTT_SERVER, MQTT_PORT);
    } else {
      Log.error(F("MQTT connection failed to [%s:%d]" CR), MQTT_SERVER, MQTT_PORT);
    }
  }
  
  return isRunning();
}

bool MQTTService::stop() {

  if (isRunning()) {
    mqttClient.disconnect();
  }
  
  return isRunning();
}

void MQTTService::publish(const char* topic, JsonObject& json) {

  // convert JSON into char array payload
  int length = json.measureLength() + 1;
  char payload[length];
  json.printTo(payload, length);

  mqttClient.publish(topic, 0, true, payload, length);
  Log.verbose(F("Message send: %s" CR), payload);
}

/**
 * MQTT callback handlers
 */

void MQTTService::onConnect(bool sessionPresent) {

  Log.verbose("CALLBACK: Connection established to mqtt broker [%s:%d]\n", MQTT_SERVER, MQTT_PORT);
}

void MQTTService::onDisconnect(AsyncMqttClientDisconnectReason reason) {

  Log.verbose("CALLBACK: Disonnected from mqtt broker [%s:%d]\n", MQTT_SERVER, MQTT_PORT);
}

void MQTTService::onPublish(uint16_t packetId) {

  Log.verbose("CALLBACK: Publish acknowledged with packetId: %d\n", packetId);
}

void MQTTService::onSubscribe(uint16_t packetId, uint8_t qos) {

  Log.verbose("CALLBACK: Subscribe acknowledged with packetId: %d\n", packetId);
}

void MQTTService::onUnsubscribe(uint16_t packetId) {

  Log.verbose("CALLBACK: Unsubscribe acknowledged with packetId: %d\n", packetId);
}

void MQTTService::onMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {

  // TODO add an expressive log message
  Log.verbose("CALLBACK: Message received\n");
}
