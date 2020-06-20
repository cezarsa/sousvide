#ifndef _SOUSVIDE_RELAY_HPP_
#define _SOUSVIDE_RELAY_HPP_

#include "mqtt.h"

class relay {
 private:
  bool state = false;
  String name;
  int pin;
  mqtt* conn;

  void refresh();

 public:
  relay(String name, int pin);
  ~relay();

  void on();
  void off();
  void toggle();

  void bindMQTT(mqtt* conn, bool pubOnly);
};

#endif