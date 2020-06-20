#ifndef _SOUSVIDE_MQTT_HPP_
#define _SOUSVIDE_MQTT_HPP_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <map>

#define MQTT_DEFAULT_PORT 1883

struct cmpCStr {
  bool operator()(const char* a, const char* b) const {
    return strcmp(a, b) < 0;
  }
};

class mqtt {
 private:
  WiFiClient wifi;
  PubSubClient pubsub;
  String baseTopic;
  std::map<const char*, std::function<void(String)>, cmpCStr> topicMap;
  unsigned long lastMQTTCheck = 0;

  void connect();
  void onMessage(char* topic, uint8_t* payload, unsigned int length);

 public:
  mqtt(String server, String topic);
  ~mqtt();

  void loop();

  void listen(String name, std::function<void(String)> callback);
  bool publish(String name, String value);
};

#endif