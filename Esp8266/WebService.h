#pragma once

#include <ESPAsyncTCP.h> // https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "Service.h"

#include "L298NHandler.h"
#include "MotorShieldHandler.h"
#include "RemoteCarHandler.h"

class WebService : public Service {

  public:

    WebService();

    ~WebService();

    bool isRunning();
    
    bool start();

    bool stop();

    AsyncCallbackWebHandler& on(const char* uri, ArRequestHandlerFunction onRequest);
    AsyncCallbackWebHandler& on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);
    AsyncCallbackWebHandler& on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload);
    AsyncCallbackWebHandler& on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload, ArBodyHandlerFunction onBody);

    bool remove(AsyncWebHandler* handler);

    ArRequestHandlerFunction notFoundFunction();

  private:

		AsyncWebServer webServer;
		
    AsyncWebSocket l298nWS;
    AsyncWebSocket remoteCarWS;

    L298NHandler l298nHandler;
    RemoteCarHandler remoteCarHandler;
    
    bool _running = false;
};
