#ifndef _MQTT_HPP_
#define _MQTT_HPP_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class mqtt {
 private:
  WiFiClient wifi;
  PubSubClient pubsub;
  String topic;
  unsigned long lastMQTTCheck = 0;

  void connect();
  void onMessage(char* topic, uint8_t* payload, unsigned int length);

 public:
  mqtt(String server, String topic);
  ~mqtt();

  void loop();
};

#endif