#pragma once

class NotFoundHandler {

  public:

    NotFoundHandler() {};

    void request(AsyncWebServerRequest *request) {

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
      Serial.printf("HTTP 404 : http://%s%s \n", request->host().c_str(), request->url().c_str());

      request->send(404, F("Page not found."));
    }
};
