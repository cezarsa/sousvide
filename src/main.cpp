#include "main.h"

#include <Arduino.h>

#include "base.h"
#include "common.h"
#include "controller.h"

const bool displayEnabled = true;

board::board()
    : pump("pump", D5),
      heater("heater", D6),
      water("water", D7),
      rotary(D0, D3, RX) {}

void board::setup() {
  base app;
  if (!app.connect()) {
    return;
  }
  mqttConn = new mqtt(app.server(), app.topic());
  pump.bindMQTT(mqttConn, false);
  heater.bindMQTT(mqttConn, false);
  water.bindMQTT(mqttConn);
  control = new controller(this);
  updates.setup(control);
  if (displayEnabled) {
    display.bind(this);
  }
  rotary.setup();
}

void board::loop() {
  updates.loop();
  if (mqttConn) {
    mqttConn->loop();
  }
  water.loop();
  if (control) {
    control->loop();
  }
  if (displayEnabled) {
    display.loop();
  }
  rotary.loop();
}

board b;

void setup() {
  b.setup();
}

void loop() {
  b.loop();
}