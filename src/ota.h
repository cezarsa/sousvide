#pragma once

#include "control_base.h"

class ota {
 private:
  controller* control;

 public:
  ota();
  ~ota();

  void setup(controller* control);
  void loop();
};
