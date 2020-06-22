#pragma once

#include "mqtt.h"

class relay {
 private:
  bool state = false;
  String name;
  int pin;
  mqtt* conn = nullptr;
  unsigned long lastRefreshCheck = 0;

  void refresh();

 public:
  relay(String name, int pin);
  ~relay();

  void on();
  void off();
  void toggle();

  void bindMQTT(mqtt* conn, bool pubOnly);
  void loop();
};
