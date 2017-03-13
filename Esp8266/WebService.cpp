#include "WebService.h"

WebService::WebService(bool enabled, bool verbose) :
  webServer(80),
  dc("/dc"),
  Service(enabled, verbose) {

  dc.onEvent(onDCEvent);
  webServer.addHandler(&dc);

  // rewrite root to default index file
  webServer.rewrite("/", DEFAULT_INDEX);
  // handle static web resources
  webServer.serveStatic("/", SPIFFS, "/www/", "max-age:600"); // cache-control 600 seconds
  // handle dynamic web resources
  webServer.on("/esp", HTTP_GET, std::bind(&ESPHandler::classRequest, espHandler, std::placeholders::_1));
  webServer.on("/list", HTTP_GET, std::bind(&FileListingHandler::classRequest, fileListingHandler, std::placeholders::_1));
  webServer.on("/scan", HTTP_GET, std::bind(&WiFiScanHandler::classRequest, wiFiScanHandler, std::placeholders::_1));
  // handle 404
  webServer.onNotFound(std::bind(&NotFoundHandler::classRequest, notFoundHandler, std::placeholders::_1));
  // start web server
  webServer.begin();
}

void WebService::onDCEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

  if (type == WS_EVT_CONNECT) {
    Serial.printf("ws[%s][%u] connected\n", server->url(), client->id());
    sendInitEvent(server, client);
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("ws[%s][%u] disconnected: %u\n", server->url(), client->id());
  } else if (type == WS_EVT_ERROR) {
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if (type == WS_EVT_PONG) {
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if (type == WS_EVT_DATA) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
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
      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);

      client->text("{'message': ['state': 'failed', 'description': 'Unexpected message']}");
    }
  }
}

void WebService::sendInitEvent(AsyncWebSocket *server, AsyncWebSocketClient *client) {

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

bool WebService::processEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, const String& message) {

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
