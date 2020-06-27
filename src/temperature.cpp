#include "temperature.h"

temp::temp(String name, int pin)
    : name(name), oneWire(pin), sensors(&oneWire) {}

temp::~temp() {}

float temp::readTemperature() {
  unsigned long now = millis();
  lastRead = now;
  lastTemp = readTemperatureRaw();
  return lastTemp;
}

float temp::readTemperatureRaw() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

void temp::bindMQTT(mqtt* conn) {
  this->conn = conn;
  sensors.begin();
}

void temp::loop() {
  if (!this->conn) {
    return;
  }
  unsigned long now = millis();
  if (lastLoop != 0 && now - lastLoop < 5000) {
    return;
  }
  lastLoop = now;

  float t;
  if (lastRead != 0 && now - lastRead < 4000) {
    t = lastTemp;
  } else {
    t = readTemperature();
  }
  this->conn->publish(name + "/temperature", String(t));
}
