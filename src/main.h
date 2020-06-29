#pragma once

#include "controller.h"
#include "mqtt.h"
#include "ota.h"
#include "relay.h"
#include "screen.h"
#include "temperature.h"

class board {
 public:
  mqtt* mqttConn = nullptr;
  relay pump, heater;
  temp water;
  controller* control = nullptr;
  ota updates;
  screen display;

  board();
  void setup();
  void loop();
};
