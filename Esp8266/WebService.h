#pragma once

#include <ESPAsyncTCP.h> // https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "Service.h"

#include "handler/ESPHandler.h"
#include "handler/FileListingHandler.h"
#include "handler/NotFoundHandler.h"
#include "handler/WiFiScanHandler.h"

class WebService : public Service {

	public:
		WebService(bool enabled, bool verbose);

  private:
    AsyncWebServer webServer;

    ESPHandler espHandler;
    FileListingHandler fileListingHandler;
    NotFoundHandler notFoundHandler;
    WiFiScanHandler wiFiScanHandler;
};
