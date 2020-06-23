#include "base.h"

#include <LittleFS.h>

#include "common.h"

base::base()
    : mqttTopic("topic",
                "mqtt topic",
                "home/kitchen/sousvide",
                MAX_PARAM_LENGTH - 1),
      mqttServer("server", "mqtt server", "192.168.1.2", MAX_PARAM_LENGTH - 1) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  Serial.begin(9600);
  logger.println("[base] Initializing...");

  wifiManager.setConfigPortalTimeout(300);
  wifiManager.setSaveConfigCallback(std::bind(&base::saveConfig, this));
  wifiManager.addParameter(&mqttServer);
  wifiManager.addParameter(&mqttTopic);
}

bool base::connect() {
  loadConfig();
  String ssid = String("sousvide-") + ESP.getChipId();
  if (!wifiManager.autoConnect(ssid.c_str())) {
    logger.println("[base] failed to connect and hit timeout");
    delay(1000);
    ESP.restart();
    return false;
  }
  saveConfig();
  digitalWrite(LED_BUILTIN, HIGH);
  logger.print("[base] Connected with IP: ");
  logger.println(WiFi.localIP());
  return true;
}

void base::saveConfig() {
  if (!LittleFS.begin()) {
    return;
  }

  auto params = wifiManager.getParameters();
  auto paramsCount = wifiManager.getParametersCount();
  for (int i = 0; i < paramsCount; i++) {
    auto param = params[i];

    File configFile = LittleFS.open(param->getID(), "w");
    if (!configFile) {
      return;
    }
    configFile.write((uint8_t*)param->getValue(), param->getValueLength());
    configFile.close();
  }
}

void base::loadConfig() {
  if (!LittleFS.begin()) {
    return;
  }

  auto params = wifiManager.getParameters();
  auto paramsCount = wifiManager.getParametersCount();
  for (int i = 0; i < paramsCount; i++) {
    auto param = params[i];

    File configFile = LittleFS.open(param->getID(), "r");
    if (!configFile) {
      return;
    }
    param->setValue(configFile.readString().c_str(), MAX_PARAM_LENGTH - 1);
    configFile.close();
  }
}

String base::topic() {
  return mqttTopic.getValue();
}

String base::server() {
  return mqttServer.getValue();
}

base::~base() {
  LittleFS.end();
}
