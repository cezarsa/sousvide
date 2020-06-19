#include "base.hpp"

#include <FS.h>
#include <LittleFS.h>

base::base()
    : mqtt(wifi),
      custom_mqtt_topic("topic",
                        "mqtt topic",
                        "home/kitchen/sousvide",
                        MAX_TOPIC_LENGTH - 1) {
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.setSaveConfigCallback(std::bind(&base::saveTopic, this));
  wifiManager.addParameter(&custom_mqtt_topic);
}

bool base::connect() {
  digitalWrite(LED_BUILTIN, LOW);
  loadTopic();
  String ssid = String("esp-") + ESP.getChipId();
  if (!wifiManager.autoConnect(ssid.c_str())) {
    Serial.println("failed to connect and hit timeout");
    delay(1000);
    ESP.restart();
    return false;
  }
  saveTopic();
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  return true;
}

void base::saveTopic() {
  if (!LittleFS.begin()) {
    return;
  }

  String mqtt_topic = custom_mqtt_topic.getValue();
  File configFile = LittleFS.open("mqtt_topic.txt", "w");
  if (!configFile) {
    return;
  }
  configFile.write((uint8_t*)mqtt_topic.c_str(), mqtt_topic.length());
  configFile.close();
}

void base::loadTopic() {
  if (!LittleFS.begin()) {
    return;
  }

  File configFile = LittleFS.open("mqtt_topic.txt", "r");
  if (!configFile) {
    return;
  }
  custom_mqtt_topic.setValue(configFile.readString().c_str(),
                             MAX_TOPIC_LENGTH - 1);
  configFile.close();
}

base::~base() {}
