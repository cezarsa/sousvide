#include <Arduino.h>

#include "base.h"
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
}

void board::loop() {
  mqttConn->loop();
  pump.loop();
  heater.loop();
  water.loop();
}

board b;

void setup() {
  b.setup();
}

void loop() {
  b.loop();
}