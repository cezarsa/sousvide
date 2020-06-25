#pragma once

#include "control_base.h"

class ota {
 private:
  controller* control = nullptr;

 public:
  ota();
  ~ota();

  void setup(controller* control);
  void loop();
};
