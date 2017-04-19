#pragma once

#include <AsyncMqttClient.h> // https://github.com/marvinroger/async-mqtt-client
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "Service.h"

class MQTTService : public Service {

	public:

		MQTTService(bool enabled);

    ~MQTTService();
    
    void publish(const char* topic, JsonObject& json);

  private:

		AsyncMqttClient mqttClient;

    static void onConnect(bool sessionPresent);

    static void onDisconnect(AsyncMqttClientDisconnectReason reason);

    static void onPublish(uint16_t packetId);

    static void onSubscribe(uint16_t packetId, uint8_t qos);

    static void onUnsubscribe(uint16_t packetId);

    static void onMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
};
