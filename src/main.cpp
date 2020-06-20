#include <Arduino.h>

#include "base.h"
#include "mqtt.h"
#include "relay.h"

class board {
 private:
  mqtt* mqttConn;
  relay pump, heater;

 public:
  board();
  void setup();
  void loop();
};

board::board() : pump("pump", D5), heater("heater", D6) {}

void board::setup() {
  base app;
  if (!app.connect()) {
    return;
  }
  mqttConn = new mqtt(app.server(), app.topic());
  pump.bindMQTT(mqttConn, false);
  heater.bindMQTT(mqttConn, false);
}

void board::loop() {
  mqttConn->loop();
}

board b;

void setup() {
  b.setup();
}

void loop() {
  b.loop();
}