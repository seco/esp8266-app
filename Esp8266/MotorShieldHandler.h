#pragma once

#include <AsyncWebSocket.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "MotorShieldDriver.h"
#include "WSHandler.h"

class MotorShieldHandler : public WSHandler {

  public:

    MotorShieldHandler(bool enabled, unsigned int pinPWM, unsigned int pinDir);

    void processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json);

    void notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast);

  private:

    MotorShieldDriver *motor;
};
