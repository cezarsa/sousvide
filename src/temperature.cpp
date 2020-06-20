#include "temperature.h"

temp::temp(String name, int pin)
    : name(name), oneWire(pin), sensors(&oneWire) {}

temp::~temp() {}

float temp::readTemperature() {
  if (!initialized) {
    sensors.begin();
  }

  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  if (tempC != DEVICE_DISCONNECTED_C) {
    initialized = true;
  }
  return tempC;
}

void temp::bindMQTT(mqtt* conn) {
  this->conn = conn;
}

void temp::loop() {
  if (!this->conn) {
    return;
  }
  unsigned long now = millis();
  if (now - lastLoop < 5000) {
    return;
  }
  lastLoop = now;
  auto t = readTemperature();
  this->conn->publish(name + "/temperature", String(t));
}
