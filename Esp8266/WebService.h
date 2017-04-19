#pragma once

#include <ESPAsyncTCP.h> // https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "Service.h"

#include "L298NHandler.h"
#include "MotorShieldHandler.h"
#include "RemoteCarHandler.h"

#include "resource/ESPHandler.h"
#include "resource/FileListingHandler.h"
#include "resource/NotFoundHandler.h"
#include "resource/WiFiScanHandler.h"

class WebService : public Service {

  public:

    WebService();

    bool isRunning();
    
    bool start();

    bool stop();

  private:

		AsyncWebServer webServer;
		
    AsyncWebSocket l298nWS;
    AsyncWebSocket remoteCarWS;

    L298NHandler l298nHandler;
    RemoteCarHandler remoteCarHandler;

    ESPHandler espHandler;
    FileListingHandler fileListingHandler;
    NotFoundHandler notFoundHandler;
    WiFiScanHandler wiFiScanHandler;
};
