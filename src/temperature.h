#pragma once

#include <DallasTemperature.h>
#include <OneWire.h>

#include "mqtt.h"

const unsigned long MIN_TEMPERATURE_READ_INTERVAL = 1000;

class temp {
 private:
  String name;
  OneWire oneWire;
  DallasTemperature sensors;
  unsigned long lastLoop = 0;
  mqtt* conn = nullptr;
  float lastTemp = DEVICE_DISCONNECTED_C;
  unsigned long lastRead = 0;
  unsigned long conversionDeadline = 0;

  float readTemperatureRaw();

 public:
  temp(String name, int pin);
  ~temp();

  void bindMQTT(mqtt* conn);
  void loop();
  float readTemperature();
};
