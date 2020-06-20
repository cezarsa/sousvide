#ifndef _SOUSVIDE_TEMPERATURE_H_
#define _SOUSVIDE_TEMPERATURE_H_

#include <DallasTemperature.h>
#include <OneWire.h>

#include "mqtt.h"

class temp {
 private:
  String name;
  OneWire oneWire;
  DallasTemperature sensors;
  bool initialized = false;
  unsigned long lastLoop = 0;
  mqtt* conn = nullptr;

 public:
  temp(String name, int pin);
  ~temp();

  void bindMQTT(mqtt* conn);
  void loop();
  float readTemperature();
};

#endif