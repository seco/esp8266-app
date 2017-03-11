#include "config.h"
#include "handler/ESPHandler.h"
#include "handler/FileListingHandler.h"
#include "handler/NotFoundHandler.h"
#include "handler/WiFiScanHandler.h"

#include <FS.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h

#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <ESP8266WiFiMulti.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src/ESP8266WiFiMulti.h
#include <ESP8266mDNS.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/ESP8266mDNS.h

ESP8266WiFiMulti wifiMulti;

#include <ESPAsyncTCP.h> // https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

AsyncWebServer webServer(80);
AsyncWebSocket led("/led");
AsyncWebSocket rc("/rc");

/**
 * Various resource handler
 */
ESPHandler espHandler;
FileListingHandler fileListingHandler;
NotFoundHandler notFoundHandler;
WiFiScanHandler wiFiScanHandler;

#define LED 15

void sendInitEvent(AsyncWebSocket *server, AsyncWebSocketClient *client) {

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["brightness"] = analogRead(LED);
  // convert JSON into char array payload
  int length = json.measureLength() + 1;
  char payload[length];
  json.printTo(payload, length);
  DEBUG_MSG("ws[%s][%u] send : %s\n", server->url(), client->id(), payload);
  client->text(payload);
}

bool processEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, const String& message) {

  // try to interpret message as JSON
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(message);
  // interrupt if message contains no valid JSON
  if (!json.success()) {
    DEBUG_MSG("Parsing message into JSON failed.\n");
        return false;
  }

  // read expected values
  int brightness = json["brightness"];
  analogWrite(LED, brightness); 

  return true;
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  
  if (type == WS_EVT_CONNECT) {
    DEBUG_MSG("ws[%s][%u] connected\n", server->url(), client->id());
    sendInitEvent(server, client);
  } else if (type == WS_EVT_DISCONNECT) {
    DEBUG_MSG("ws[%s][%u] disconnected: %u\n", server->url(), client->id());
  } else if (type == WS_EVT_ERROR) {
    DEBUG_MSG("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if (type == WS_EVT_PONG) {
    DEBUG_MSG("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
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
      DEBUG_MSG("ws[%s][%u] received : %s\n", server->url(), client->id(), message.c_str());
      processEvent(server, client, message);
    } else {
      DEBUG_MSG("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);

      client->text("{'message': ['state': 'failed', 'description': 'Unexpected message']}");
    }
  }
}

void setup() {

  // setup serial
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  DEBUG_MSG("\n");
  DEBUG_MSG("Booting...\n");

  // do various setup calls ...
  setupBoard();
  
  setupWiFi();
  setupFileSystem();
  setupWebServer();
}

void setupBoard() {

  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
}

void setupFileSystem() {

  if (!SPIFFS.begin()) {
    DEBUG_MSG("Couldn't mount file system!\n");
  }
}

void setupWiFi() {

  // TODO take care of a clean WiFi reset

  // disconnect from any previous WiFi connect
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  // set WiFi mode
  WiFi.mode(WIFI_MODE);
  delay(300);

  WiFi.persistent(false);
  WiFi.hostname(HOST_NAME);

  // create access point (AP)
  #if (WIFI_MODE == WIFI_AP || WIFI_MODE == WIFI_AP_STA)
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWD);
    delay(300);
    DEBUG_MSG("IP address of AP is: %s\n", WiFi.softAPIP().toString().c_str());
  #endif
  
  // connect to WiFi
  #if (WIFI_MODE == WIFI_STA || WIFI_MODE == WIFI_AP_STA)
    // add access point configurations
    wifiMulti.addAP(WIFI_SSID_1, WIFI_PASSWD_1);
    wifiMulti.addAP(WIFI_SSID_2, WIFI_PASSWD_2);
    // try to connect to WiFi
    DEBUG_MSG("Trying to connect WiFi ");
    uint8_t i = 0;
    while (wifiMulti.run() != WL_CONNECTED && i++ < 20) { // retry 20 times = 10 seconds
      delay(300);
      DEBUG_MSG(".");
    }
    DEBUG_MSG("\n");
    if (i > 20) {
      DEBUG_MSG("Couldn't connect to any WiFi. Please check your WiFi availability / accessibility and restart.\n");
    } else {
      DEBUG_MSG("WiFi successful connected. IP address is: %s\n", WiFi.localIP().toString().c_str());
    }
  #endif
  
  // enable multicast DNS
  #ifdef HOST_NAME
    // add <domain name>.local (mDNS)
    if (MDNS.begin(HOST_NAME)) {
      // add service
      MDNS.addService("http", "tcp", 80);
      //MDNS.addService("ws", "tcp", 81);
      DEBUG_MSG("Open [http://%s.local] in your browser.\n", HOST_NAME);
    } else {
      DEBUG_MSG("Couldn't set up [http://%s.local]\n", HOST_NAME);
    }
  #endif
}

void setupWebServer() {

  // handle WebSockets
  led.onEvent(onWsEvent);
  webServer.addHandler(&led);
  rc.onEvent(onWsEvent);
  webServer.addHandler(&rc);
  
  // handle dynamic web resources
  webServer.on("/esp", HTTP_ANY, std::bind(&ESPHandler::classRequest, espHandler, std::placeholders::_1));
  webServer.on("/list", HTTP_ANY, std::bind(&FileListingHandler::classRequest, fileListingHandler, std::placeholders::_1));
  webServer.on("/scan", HTTP_ANY, std::bind(&WiFiScanHandler::classRequest, wiFiScanHandler, std::placeholders::_1));
  
  // rewrite root to default index file
  webServer.rewrite("/", DEFAULT_INDEX);
  // handle static web resources
  webServer.serveStatic("/", SPIFFS, "/www/", "max-age:600"); // cache-control 600 seconds
  // handle 404
  webServer.onNotFound(std::bind(&NotFoundHandler::classRequest, notFoundHandler, std::placeholders::_1));

  // start web server
  webServer.begin();
}

void loop() {
  // do something else
}
