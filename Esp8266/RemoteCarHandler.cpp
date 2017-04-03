#include "RemoteCarHandler.h"

RemoteCarHandler::RemoteCarHandler(bool enabled) {

  motorA = new L298NMotorDriver(enabled, PIN_L298N_ENA, PIN_L298N_IN1, PIN_L298N_IN2);
  motorA->setSpeed(0);
  motorB = new L298NMotorDriver(enabled, PIN_L298N_ENB, PIN_L298N_IN3, PIN_L298N_IN4);
  motorB->setSpeed(0);

  Log.verbose(F("Setup for remote control done." CR));
};

void RemoteCarHandler::processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json) {
  
  uint8_t speed = json["speed"];
  String command = json["command"];

  switch (command.charAt(0)) {
    case 'L': // [L]eft
      motorA->apply(-speed);
      motorB->apply(speed);
      break;
    case 'F': // [F]orward
      motorA->apply(speed);
      motorB->apply(speed);
      break;
    case 'R': // [R]ight
      motorA->apply(speed);
      motorB->apply(-speed);
      break;
    case 'B': // [B]ackward
      motorA->apply(-speed);
      motorB->apply(-speed);
      break;
    case 'S': // [S]top
      motorA->setSpeed(0);
      motorB->setSpeed(0);
      break;
    default:  // invalid
      break;
  }

  notify(ws, client, "message", true);
}

void RemoteCarHandler::notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast = false) {

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["type"] = type;
  json["clientId"] = client->id();
  json["speedA"] = motorA->getSpeed();
  json["speedB"] = motorB->getSpeed();

  send(ws, client, &json, broadcast);
}
