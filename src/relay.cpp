#include "relay.h"

#include <Arduino.h>

#include "common.h"

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

void relay::refresh() {
  digitalWrite(pin, state ? LOW : HIGH);
  if (conn) {
    conn->publish(name + "/state", String(state));
  }
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
