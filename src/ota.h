#pragma once

#include "controller.h"

class ota {
 private:
  controller* control = nullptr;

 public:
  ota();
  ~ota();

  void setup(controller* control);
  void loop();
};
