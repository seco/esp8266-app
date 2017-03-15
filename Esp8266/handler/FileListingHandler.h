#pragma once

#include <FS.h>

#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

class FileListingHandler {
  public :
    FileListingHandler(){};

    void classRequest(AsyncWebServerRequest *request) {

      AsyncJsonResponse *response = new AsyncJsonResponse();
      JsonObject& json = response->getRoot();
      JsonArray& files = json.createNestedArray("files");
      // enumerate files
      Dir dir = SPIFFS.openDir("/");
      while (dir.next()) {
        String name = String(dir.fileName());
        String size = formatBytes(dir.fileSize());
        JsonObject& entry = files.createNestedObject();
        entry[F("name")] = name;
        entry[F("size")] = size;
        Serial.printf("File: name=%s, size=%s", name.c_str(), size.c_str());
      }
      response->setLength();
      request->send(response);
    }

    String formatBytes(size_t bytes) {

      if (bytes < 1024){
        return String(bytes) + "B";
      } else if (bytes < (1024 * 1024)){
        return String(bytes/1024.0) + "KB";
      } else if (bytes < (1024 * 1024 * 1024)){
        return String(bytes/1024.0/1024.0) + "MB";
      } else {
        return String(bytes/1024.0/1024.0/1024.0) + "GB";
      }
    }
};
