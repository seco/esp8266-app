#include "MQTTService.h"

MQTTService::MQTTService(bool enabled) : Service(enabled) {

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
  Log.verbose(F("Connecting to mqtt broker [%s:%d] ..." CR), MQTT_SERVER, MQTT_PORT);
  mqttClient.connect();
}

MQTTService::~MQTTService() {
  mqttClient.disconnect();
}

void MQTTService::publish(const char* topic, JsonObject& json) {

  // convert JSON into char array payload
  int length = json.measureLength() + 1;
  char payload[length];
  json.printTo(payload, length);

  mqttClient.publish(topic, 0, true, payload, length);
  Log.verbose("message send");
}

/**
 * MQTT callback handlers
 */

void MQTTService::onConnect(bool sessionPresent) {

  Log.verbose("Connection established to mqtt broker [%s:%d]\n", MQTT_SERVER, MQTT_PORT);
}

void MQTTService::onDisconnect(AsyncMqttClientDisconnectReason reason) {

  Log.verbose("Disonnected from mqtt broker [%s:%d]\n", MQTT_SERVER, MQTT_PORT);
}

void MQTTService::onPublish(uint16_t packetId) {

  Log.verbose("Publish acknowledged with packetId: %d", packetId);
}

void MQTTService::onSubscribe(uint16_t packetId, uint8_t qos) {

  Log.verbose("Subscribe acknowledged with packetId: %d", packetId);
}

void MQTTService::onUnsubscribe(uint16_t packetId) {

  Log.verbose("Unsubscribe acknowledged with packetId: %d", packetId);
}

void MQTTService::onMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {

  // TODO add an expressive log message
  Log.verbose("Message received");
}
