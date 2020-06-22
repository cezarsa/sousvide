#pragma once

#include "control_base.h"
#include "mqtt.h"
#include "ota.h"
#include "relay.h"
#include "temperature.h"

class board {
 public:
  mqtt* mqttConn = nullptr;
  relay pump, heater;
  temp water;
  controller* control = nullptr;
  ota updates;

  board();
  void setup();
  void loop();
};
