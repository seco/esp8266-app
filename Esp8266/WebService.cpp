#include "WebService.h"

WebService::WebService(bool enabled, bool verbose) :
  webServer(80),
  dc("/dc"),
  Service(enabled, verbose) {

  dc.onEvent(std::bind(&WSHandler::onEvent, wsHandler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
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
