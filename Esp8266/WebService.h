#pragma once

#include <ESPAsyncTCP.h> // https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "Service.h"

#include "handler/WSHandler.h"

#include "handler/ESPHandler.h"
#include "handler/FileListingHandler.h"
#include "handler/NotFoundHandler.h"
#include "handler/WiFiScanHandler.h"

class WebService : public Service {

	public:
		WebService(bool enabled, bool verbose);

    static void onDCEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    static void sendInitEvent(AsyncWebSocket *server, AsyncWebSocketClient *client);
    static bool processEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, const String& message);

  private:
    AsyncWebServer webServer;
    AsyncWebSocket dc;

    WSHandler wsHandler;
    ESPHandler espHandler;
    FileListingHandler fileListingHandler;
    NotFoundHandler notFoundHandler;
    WiFiScanHandler wiFiScanHandler;
};
