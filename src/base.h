#ifndef _SOUSVIDE_BASE_H_
#define _SOUSVIDE_BASE_H_

#include <PubSubClient.h>
#include <WiFiManager.h>

const int MAX_PARAM_LENGTH = 64;

class base {
 private:
  WiFiManager wifiManager;
  WiFiManagerParameter mqttTopic;
  WiFiManagerParameter mqttServer;

  void saveConfig();
  void loadConfig();

 public:
  base();
  ~base();

  bool connect();
  String topic();
  String server();
};

#endif