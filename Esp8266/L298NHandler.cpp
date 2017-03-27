#include "L298NHandler.h"

L298NHandler::L298NHandler(bool enabled, unsigned int pinPWM, unsigned int pinInputA, unsigned int pinInputB) {

  motor = new L298NMotorDriver(enabled, pinPWM, pinInputA, pinInputB);
  motor->setSpeed(0);
};

void L298NHandler::processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json) {

  int speed = json["speed"];
  motor->setSpeed(speed);

  notify(ws, client, "message", true);
}

void L298NHandler::notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast = false) {

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["type"] = type;
  json["clientId"] = client->id();
  json["speed"] = motor->getSpeed();

  send(ws, client, &json, broadcast);
}
