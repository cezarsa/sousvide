#pragma once

#include <Rotary.h>

class input {
 private:
  int clk, dt, sw;
  float step = 0.5f, minValue = 30.f, maxValue = 80.f;
  float counter = minValue;
  float lastPosition = 0.f;
  Rotary encoder;

  void encoderISR();

 public:
  input(int sw, int dt, int clk);
  ~input();
  void setup();
  void loop();

  float getCounter() { return counter; }
};
