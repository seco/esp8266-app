#pragma once

#include <AsyncWebSocket.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "L298NMotorDriver.h"
#include "WSHandler.h"

class RemoteCarHandler : public WSHandler {

  public:

    RemoteCarHandler(bool enabled);

    void processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json);

    void notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast);

  private:

    L298NMotorDriver *motorA;
    L298NMotorDriver *motorB;
};
