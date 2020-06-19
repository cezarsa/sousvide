#ifndef _BASE_HPP_
#define _BASE_HPP_

#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

const int MAX_TOPIC_LENGTH = 64;

class base {
 private:
  WiFiClient wifi;
  MqttClient mqtt;
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