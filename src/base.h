#ifndef _BASE_HPP_
#define _BASE_HPP_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>

const int MAX_TOPIC_LENGTH = 64;

class base {
 private:
  WiFiClient wifi;
  PubSubClient mqtt;
  WiFiManager wifiManager;
  WiFiManagerParameter custom_mqtt_topic;

  void saveTopic();
  void loadTopic();

 public:
  base();
  ~base();

  bool connect();
};

#endif