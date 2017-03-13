#include "Esp8266.h"

Esp8266 *esp8266;

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println(F("booting ESP8266 ..."));

  esp8266 = new Esp8266(); 
}

void loop() {
	esp8266->run();
}
