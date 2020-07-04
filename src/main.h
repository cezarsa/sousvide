#pragma once

#include "controller.h"
#include "mqtt.h"
#include "ota.h"
#include "relay.h"
#include "screen.h"
#include "temperature.h"
#include "input.h"

class board {
 public:
  mqtt* mqttConn = nullptr;
  relay pump, heater;
  temp water;
  controller* control = nullptr;
  ota updates;
  screen display;
  input rotary;

  board();
  void setup();
  void loop();
};
