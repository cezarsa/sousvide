#include <Arduino.h>

#include "base.h"

base app;

void setup() {
  if (!app.connect()) {
    return;
  }
}

void loop() {}