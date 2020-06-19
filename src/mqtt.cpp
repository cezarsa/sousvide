#include "mqtt.h"

void baseOnMessage(char* topic, uint8_t* payload, unsigned int length) {}

mqtt::mqtt(String server, String topic) : pubsub(wifi) {
  this->topic = topic;
  pubsub.setServer(server.c_str(), 1883);
  pubsub.setCallback([=](char* topic, uint8_t* payload, unsigned int length) {
    this->onMessage(topic, payload, length);
  });
}

mqtt::~mqtt() {}

void mqtt::onMessage(char* topic, uint8_t* payload, unsigned int length) {}

void mqtt::loop() {
  connect();
  pubsub.loop();
}

void mqtt::connect() {
  unsigned long now = millis();
  if (now - lastMQTTCheck < 5000) {
    return;
  }
  lastMQTTCheck = now;
  if (pubsub.connected()) {
    return;
  }
  String clientId = String("ESP8266Client-") + ESP.getChipId();

  Serial.print("Attempting MQTT connection...");

  if (pubsub.connect(clientId.c_str())) {
    String subscribeTopic = topic;
    subscribeTopic += "/+/set";
    Serial.print("connected, subscribing ");
    Serial.println(subscribeTopic);
    pubsub.subscribe(subscribeTopic.c_str());
  } else {
    Serial.print("failed, rc=");
    Serial.print(pubsub.state());
    Serial.println(" try again in 5 seconds");
  }
}