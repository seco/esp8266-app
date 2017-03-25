#pragma once

#include <AsyncWebSocket.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "../L298NMotorDriver.h"

class L298NHandler {
  public:

    L298NMotorDriver *motor;

    L298NHandler(bool enabled, unsigned int pinPWM, unsigned int pinInput1, unsigned int pinInput2) {

      motor = new L298NMotorDriver(enabled, pinPWM, pinInput1, pinInput2);
      motor->setSpeed(0);
    };

    void onEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

      AwsFrameInfo *info = (AwsFrameInfo*)arg;
      switch (type) {
        case WS_EVT_CONNECT:
          Log.verbose(F("ws[%s][%u] connected" CR), ws->url(), client->id());
          notify(ws, client, "connect", false);
          break;
        case WS_EVT_DISCONNECT:
          Log.verbose(F("ws[%s][%u] disconnected: %u" CR), ws->url(), client->id());
          break;
        case WS_EVT_ERROR:
          Log.error(F("ws[%s][%u] error(%u): %s" CR), ws->url(), client->id(), *((uint16_t*)arg), (char*)data);
          break;
        case WS_EVT_PONG:
          Log.verbose(F("ws[%s][%u] pong[%u]: %s" CR), ws->url(), client->id(), len, (len)?(char*)data:"");
          break;
        case WS_EVT_DATA:
          // process data if it's a single frame and all payload is available and data contains only text
          if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            // convert data into char array
            char buffer[info->len + 1];
            strncpy(buffer, (char*)data, info->len);
            // make certain that the message ends with a /0 terminator
            buffer[info->len] = '\0';

            //String message = String(buffer);

            //Log.verbose("ws[%s][%u] received : %s\n", ws->url(), client->id(), message.c_str());
            processMessage(ws, client, buffer);
          } else {
            // TODO
          }
          break;
        default:
          Log.verbose(F("ws[%s][%u] frame[%u] %s[%llu - %llu]" CR), ws->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
          // TODO send a response to the client
          break;
        }
    }

    bool processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, char* message) {

      // try to interpret message as JSON
      DynamicJsonBuffer jsonBuffer;
      JsonObject &json = jsonBuffer.parseObject(message);
      // interrupt if message contains no valid JSON
      if (!json.success()) {
        Log.error(F("Parsing message into JSON failed." CR));
        return false;
      }

      // TODO: workaround - replace by Log.verbose(...)
      Serial.printf("V: ws[%s][%u] received : %s\n", ws->url(), client->id(), String(message).c_str());

      String uuid = json["uuid"];
      int speed = json["speed"];
      motor->setSpeed(speed);

      notify(ws, client, "message", true);

      return true;
    }

    void notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast = false) {

      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
      json["type"] = type;
      json["clientId"] = client->id();
      json["speed"] = motor->getSpeed();

      send(ws, client, &json, broadcast);
    }

    void send(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject *json, bool broadcast = false) {

      int length = json->measureLength() + 1;
      char payload[length];
      json->printTo(payload, length);
      if (broadcast) {
        ws->textAll(payload);
      } else {
        client->text(payload);
      }
    }
};
