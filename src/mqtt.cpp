#include "mqtt.h"

mqtt::mqtt(String server, String topic) : pubsub(wifi), baseTopic(topic) {
  Serial.printf("[mqtt] Initialize, server: %s, topic: %s\n", server.c_str(),
                topic.c_str());

  IPAddress serverIP;
  auto isIP = serverIP.fromString(server);
  if (isIP) {
    pubsub.setServer(serverIP, MQTT_DEFAULT_PORT);
  } else {
    String* serverCopy = new String(server);
    pubsub.setServer(serverCopy->c_str(), MQTT_DEFAULT_PORT);
  }
  pubsub.setCallback(
      [this](char* topic, uint8_t* payload, unsigned int length) {
        this->onMessage(topic, payload, length);
      });
}

mqtt::~mqtt() {}

void mqtt::onMessage(char* topic, uint8_t* payload, unsigned int length) {
  String topicStr(topic);
  if (!topicStr.endsWith("/set")) {
    return;
  }

  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = 0;

  Serial.printf("[mqtt] message received in \"%s\": \"%s\"\n", topic, message);

  if (topicStr.indexOf(baseTopic) != 0) {
    return;
  }

  auto name = topicStr.substring(baseTopic.length());
  Serial.printf("[mqtt] parsed name \"%s\"\n", name.c_str());

  if (topicMap.find(name.c_str()) == topicMap.end()) {
    return;
  }

  topicMap[name.c_str()](String(message));
}

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

  Serial.println("[mqtt] Attempting MQTT connection...");

  if (pubsub.connect(clientId.c_str())) {
    String subscribeTopic = baseTopic;
    subscribeTopic += "/#";
    Serial.printf("[mqtt] connected, subscribing %s\n", subscribeTopic.c_str());
    pubsub.subscribe(subscribeTopic.c_str());
  } else {
    Serial.printf("[mqtt] failed, rc=%d try again in 5 seconds\n",
                  pubsub.state());
  }
}

void mqtt::listen(String name, std::function<void(String)> callback) {
  String* toRegister = new String(String("/") + name + "/set");
  Serial.printf("[mqtt] registering topic \"%s\"\n", toRegister->c_str());
  topicMap[toRegister->c_str()] = callback;
}

bool mqtt::publish(String name, String value) {
  auto pubTopic = baseTopic + "/" + name;
  return pubsub.publish(pubTopic.c_str(), value.c_str());
}