#include "temperature.h"

temp::temp(String name, int pin)
    : name(name), oneWire(pin), sensors(&oneWire) {}

temp::~temp() {}

float temp::readTemperature() {
  unsigned long now = millis();
  if (lastRead != 0 && (now - lastRead < MIN_TEMPERATURE_READ_INTERVAL)) {
    return lastTemp;
  }
  lastRead = now;

  for (int i = 0; i < 2; i++) {
    lastTemp = readTemperatureRaw();
    initialized = lastTemp != DEVICE_DISCONNECTED_C;
    if (initialized) {
      break;
    }
  }

  return lastTemp;
}

float temp::readTemperatureRaw() {
  if (!initialized) {
    sensors.begin();
  }

  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
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
