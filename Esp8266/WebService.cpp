#include "WebService.h"

WebService::WebService() :
  webServer(80),
  l298nWS("/L298N"),
  remoteCarWS("/rc"),
  l298nHandler(true, PIN_L298N_ENA, PIN_L298N_IN1, PIN_L298N_IN2),
  remoteCarHandler(true) {}

WebService::~WebService() {
  stop();
}

bool WebService::isRunning() {
  return _running;
}

bool WebService::start() {

  if (!isRunning()) {
    // handle web sockets
    l298nWS.onEvent(std::bind(&L298NHandler::onEvent, l298nHandler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
    webServer.addHandler(&l298nWS);

    remoteCarWS.onEvent(std::bind(&RemoteCarHandler::onEvent, remoteCarHandler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
    webServer.addHandler(&remoteCarWS);

    // rewrite root to default index file
    webServer.rewrite("/", DEFAULT_INDEX);
    // handle static web resources
    webServer.serveStatic("/", SPIFFS, "/www/", "max-age:600"); // cache-control 600 seconds
    // handle 404
    webServer.onNotFound(notFoundFunction());
    // start web server
    webServer.begin();
  }
  Log.verbose("WebServer started.\n");
  
  return true;
}

bool WebService::stop() {
  
  if (isRunning()) {
    webServer.reset();
    _running = false;
  }

  return isRunning();
}

AsyncCallbackWebHandler& WebService::on(const char* uri, ArRequestHandlerFunction onRequest) {
  return webServer.on(uri, onRequest);
}

AsyncCallbackWebHandler& WebService::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest) {
  return webServer.on(uri, method, onRequest);
}

AsyncCallbackWebHandler& WebService::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload) {
  return webServer.on(uri, method, onRequest, onUpload);
}

AsyncCallbackWebHandler& WebService::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload, ArBodyHandlerFunction onBody) {
  return webServer.on(uri, method, onRequest, onUpload, onBody);
}

bool WebService::remove(AsyncWebHandler* handler) {
  return webServer.removeHandler(handler);
}

ArRequestHandlerFunction WebService::notFoundFunction() {
  
  return [](AsyncWebServerRequest *request) {
    String method = F("UNKNOWN");
    if (request->method() == HTTP_GET)
      method = F("GET");
    else if (request->method() == HTTP_POST)
      method = F("POST");
    else if (request->method() == HTTP_DELETE)
      method = F("DELETE");
    else if (request->method() == HTTP_PUT)
      method = F("PUT");
    else if (request->method() == HTTP_PATCH)
      method = F("PATCH");
    else if (request->method() == HTTP_HEAD)
      method = F("HEAD");
    else if (request->method() == HTTP_OPTIONS)
      method = F("OPTIONS");
    
    Log.verbose(F("HTTP 404 : http://%s%s" CR), request->host().c_str(), request->url().c_str());

    request->send(404, F("Page not found."));
  };
}

