#pragma once

#include <Rotary.h>

class input {
 private:
  int clk, dt, sw;
  int counter = 0;
  int lastPosition = 0;
  Rotary encoder;

  void encoderISR();

 public:
  input(int sw, int dt, int clk);
  ~input();
  void setup();
  void loop();
};
