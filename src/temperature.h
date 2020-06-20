#ifndef _SOUSVIDE_TEMPERATURE_HPP_
#define _SOUSVIDE_TEMPERATURE_HPP_

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
  mqtt* conn;

 public:
  temp(String name, int pin);
  ~temp();

  void bindMQTT(mqtt* conn);
  void loop();
  float readTemperature();
};

#endif