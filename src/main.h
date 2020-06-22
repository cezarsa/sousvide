#pragma once

#include "control_base.h"
#include "mqtt.h"
#include "relay.h"
#include "temperature.h"

class board {
 public:
  mqtt* mqttConn = nullptr;
  relay pump, heater;
  temp water;
  controller* control = nullptr;

  board();
  void setup();
  void loop();
};
