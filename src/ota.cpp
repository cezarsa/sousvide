#include "ota.h"

#include <ArduinoOTA.h>

#include "common.h"

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

    logger.println("[ota] Start updating " + type);
    control->shutdown();
  });
  ArduinoOTA.onEnd([]() { logger.println("\n[ota] Success"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    logger.printf("[ota] Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    logger.printf("[ota] Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      logger.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      logger.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      logger.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      logger.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      logger.println("End Failed");
    }
  });
  ArduinoOTA.begin(true);
}

void ota::loop() {
  ArduinoOTA.handle();
}