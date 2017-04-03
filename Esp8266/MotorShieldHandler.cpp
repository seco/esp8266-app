#include "MotorShieldHandler.h"

MotorShieldHandler::MotorShieldHandler(bool enabled, unsigned int pinPWM, unsigned int pinDir) {

  motor = new MotorShieldDriver(enabled, pinPWM, pinDir);
  motor->setSpeed(0);
};

void MotorShieldHandler::processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json) {

  int speed = json["speed"];
  motor->setSpeed(speed);

  notify(ws, client, "message", true);
}

void MotorShieldHandler::notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast = false) {

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["type"] = type;
  json["clientId"] = client->id();
  json["speed"] = motor->getSpeed();

  send(ws, client, &json, broadcast);
}
