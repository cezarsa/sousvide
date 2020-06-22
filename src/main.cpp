#include "main.h"

#include <Arduino.h>

#include "base.h"
#include "control_pid.h"

board::board() : pump("pump", D5), heater("heater", D6), water("water", D7) {}

void board::setup() {
  base app;
  if (!app.connect()) {
    return;
  }
  mqttConn = new mqtt(app.server(), app.topic());
  pump.bindMQTT(mqttConn, false);
  heater.bindMQTT(mqttConn, false);
  water.bindMQTT(mqttConn);
  control = new pidController(this);
  updates.setup(control);
}

void board::loop() {
  updates.loop();
  if (mqttConn) {
    mqttConn->loop();
  }
  pump.loop();
  heater.loop();
  water.loop();
  if (control) {
    control->loop();
  }
}

board b;

void setup() {
  b.setup();
}

void loop() {
  b.loop();
}