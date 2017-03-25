#pragma once

#include <ESPAsyncTCP.h> // https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "Service.h"

#include "ws/L298NHandler.h"

#include "resource/ESPHandler.h"
#include "resource/FileListingHandler.h"
#include "resource/NotFoundHandler.h"
#include "resource/WiFiScanHandler.h"

class WebService : public Service {

	public:
		WebService(bool enabled);

  private:
    AsyncWebServer webServer;
    AsyncWebSocket l298nWS;

    L298NHandler l298nHandler;
    
    ESPHandler espHandler;
    FileListingHandler fileListingHandler;
    NotFoundHandler notFoundHandler;
    WiFiScanHandler wiFiScanHandler;
};
