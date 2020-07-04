#include "input.h"

#include <FunctionalInterrupt.h>

#include "common.h"

input::input(int sw, int dt, int clk)
    : clk(clk), dt(dt), sw(sw), encoder(dt, clk) {}

input::~input() {}

void input::encoderISR() {
  unsigned char result = encoder.process();
  if (result == DIR_CW) {
    counter++;
  } else if (result == DIR_CCW) {
    counter--;
  }
}

void input::setup() {
  pinMode(clk, INPUT_PULLUP);
  pinMode(dt, INPUT_PULLUP);
  attachInterrupt(clk, std::bind(&input::encoderISR, this), CHANGE);
  attachInterrupt(dt, std::bind(&input::encoderISR, this), CHANGE);
}

void input::loop() {
  if (lastPosition != counter) {
    logger.println(counter);
    lastPosition = counter;
  }
}