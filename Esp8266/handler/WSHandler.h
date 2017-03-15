#pragma once

#include <AsyncWebSocket.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

class WSHandler {
  public :
    WSHandler(){};

    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

      AwsFrameInfo *info = (AwsFrameInfo*)arg;
      switch (type) {
        case WS_EVT_CONNECT:
          Serial.printf("ws[%s][%u] connected\n", server->url(), client->id());
          sendInitEvent(server, client);
          break;
        case WS_EVT_DISCONNECT:
          Serial.printf("ws[%s][%u] disconnected: %u\n", server->url(), client->id());
          break;
        case WS_EVT_ERROR:
          Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
          break;
        case WS_EVT_PONG:
          Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
          break;
        case WS_EVT_DATA:
          // process message if it's a single frame, all data is available and message contains text
          if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            // convert data into message string
            String message;
            // start an explicit sub block to free unused memory asap
            {
              char buffer[info->len + 1];
              strncpy(buffer, (char*)data, info->len);
              // make certain that the message ends with a /0 terminator
              buffer[info->len] = '\0';
              message = String(buffer);
            }
            Serial.printf("ws[%s][%u] received : %s\n", server->url(), client->id(), message.c_str());

            processEvent(server, client, message);
          } else {
            // TODO
          }
          break;
        default:
          Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
          client->text("{'message': ['state': 'failed', 'description': 'Unexpected message']}");
          break;
        }
    }

    void sendInitEvent(AsyncWebSocket *server, AsyncWebSocketClient *client) {

      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();

      json["speed"] = 255; //analogRead(MOTOR_A_PWM);
      // convert JSON into char array payload
      int length = json.measureLength() + 1;
      char payload[length];
      json.printTo(payload, length);
      Serial.printf("ws[%s][%u] send : %s\n", server->url(), client->id(), payload);
      client->text(payload);
    }

    bool processEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, const String& message) {

      // try to interpret message as JSON
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(message);
      // interrupt if message contains no valid JSON
      if (!json.success()) {
        Serial.println("Warning: Parsing message into JSON failed.");
        return false;
      }

      int speed = json["speed"];
      Serial.printf("Speed is %d\n", speed);

      return true;
    }
};
