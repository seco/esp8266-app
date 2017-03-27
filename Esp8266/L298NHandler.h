#pragma once

#include <AsyncWebSocket.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "L298NMotorDriver.h"
#include "WSHandler.h"

class L298NHandler : public WSHandler {

  public:

    L298NHandler(bool enabled, unsigned int pinPWM, unsigned int pinInputA, unsigned int pinInputB);

    void processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json);

    void notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast);

  private:

    L298NMotorDriver *motor;
};
