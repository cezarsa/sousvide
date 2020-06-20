#ifndef _SOUSVIDE_MAIN_H_
#define _SOUSVIDE_MAIN_H_

#include "mqtt.h"
#include "relay.h"
#include "temperature.h"

class board {
 private:
  mqtt* mqttConn;
  relay pump, heater;
  temp water;

 public:
  board();
  void setup();
  void loop();
};

#endif