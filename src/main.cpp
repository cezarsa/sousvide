#include <Arduino.h>

#include "base.h"
#include "mqtt.h"

mqtt* mqttConn;

void setup() {
  base app;
  if (!app.connect()) {
    return;
  }
  mqttConn = new mqtt(app.server(), app.topic());
}

void loop() {
  mqttConn->loop();
}