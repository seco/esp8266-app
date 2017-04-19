#include "WebService.h"

WebService::WebService() :
  webServer(80),
  l298nWS("/L298N"),
  remoteCarWS("/rc"),
  l298nHandler(true, PIN_L298N_ENA, PIN_L298N_IN1, PIN_L298N_IN2),
  remoteCarHandler(true) {
    
  // handle web sockets
  l298nWS.onEvent(std::bind(&L298NHandler::onEvent, l298nHandler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
  webServer.addHandler(&l298nWS);

  remoteCarWS.onEvent(std::bind(&RemoteCarHandler::onEvent, remoteCarHandler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
  webServer.addHandler(&remoteCarWS);

  // rewrite root to default index file
  webServer.rewrite("/", DEFAULT_INDEX);
  // handle static web resources
  webServer.serveStatic("/", SPIFFS, "/www/", "max-age:600"); // cache-control 600 seconds
  // handle dynamic web resources
  webServer.on("/esp", HTTP_GET, std::bind(&ESPHandler::request, espHandler, std::placeholders::_1));
  webServer.on("/list", HTTP_GET, std::bind(&FileListingHandler::request, fileListingHandler, std::placeholders::_1));
  webServer.on("/scan", HTTP_GET, std::bind(&WiFiScanHandler::request, wiFiScanHandler, std::placeholders::_1));
  // handle 404
  webServer.onNotFound(std::bind(&NotFoundHandler::request, notFoundHandler, std::placeholders::_1));    
}

bool WebService::isRunning() {
  return true;
}

bool WebService::start() {
  
  // start web server
  webServer.begin();

  //Log.verbose(F("WebServer started successful." CR));
  
  return true;
}

bool WebService::stop() {
  return true;
}
