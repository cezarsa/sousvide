#include "mqtt.h"

void baseOnMessage(char* topic, uint8_t* payload, unsigned int length) {}

mqtt::mqtt(String server, String topic) : pubsub(wifi) {
  this->topic = topic;
  Serial.printf("MQTT initialize, server: %s, topic: %s\n", server.c_str(),
                topic.c_str());

  IPAddress serverIP;
  auto isIP = serverIP.fromString(server);
  if (isIP) {
    pubsub.setServer(serverIP, MQTT_DEFAULT_PORT);
  } else {
    pubsub.setServer(server.c_str(), MQTT_DEFAULT_PORT);
  }
  pubsub.setCallback(
      [this](char* topic, uint8_t* payload, unsigned int length) {
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

  Serial.println("Attempting MQTT connection...");

  if (pubsub.connect(clientId.c_str())) {
    String subscribeTopic = topic;
    subscribeTopic += "/+/set";
    Serial.printf("connected, subscribing %s\n", subscribeTopic.c_str());
    pubsub.subscribe(subscribeTopic.c_str());
  } else {
    Serial.printf("failed, rc=%d try again in 5 seconds\n", pubsub.state());
  }
}