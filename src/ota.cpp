#include "ota.h"

#include <ArduinoOTA.h>

ota::ota() {}

ota::~ota() {}

void ota::setup(controller* control) {
  ArduinoOTA.setHostname("sousvide");
  ArduinoOTA.setRebootOnSuccess(true);

  ArduinoOTA.onStart([=]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {
      type = "filesystem";
    }

    Serial.println("[ota] Start updating " + type);
    control->shutdown();
  });
  ArduinoOTA.onEnd([]() { Serial.println("\n[ota] Success"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("[ota] Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("[ota] Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin(true);
}

void ota::loop() {
  ArduinoOTA.handle();
}