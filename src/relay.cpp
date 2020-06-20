#include "relay.h"

#include <Arduino.h>

relay::relay(String name, int pin) : name(name), pin(pin) {
  pinMode(pin, OUTPUT);
  refresh();
}

relay::~relay() {}

void relay::on() {
  state = true;
  refresh();
}

void relay::off() {
  state = false;
  refresh();
}

void relay::toggle() {
  state = !state;
  refresh();
}

void relay::refresh() {
  Serial.printf("[relay] updating %s - state %d\n", name.c_str(), state);
  digitalWrite(pin, state ? LOW : HIGH);
  if (conn) {
    conn->publish(name + "/state", String(state));
  }
}

void relay::loop() {
  unsigned long now = millis();
  if (now - lastRefreshCheck < 5000) {
    return;
  }
  lastRefreshCheck = now;
  refresh();
}

void relay::bindMQTT(mqtt* conn, bool pubOnly) {
  this->conn = conn;
  if (pubOnly) {
    return;
  }

  conn->listen(name, [this](String value) {
    this->state = value.equalsIgnoreCase("true") || value.equalsIgnoreCase("1");
    this->refresh();
  });
}
