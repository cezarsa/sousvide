#pragma once

#include <Arduino.h>

#include "control_base.h"

const int PID_EVALUATE_INTERVAL = 1000;

class pidController : public controller {
 private:
  board* b = nullptr;
  unsigned long lastLoop = 0, lastCheck = 0;
  unsigned long lastErrorNotify = 0;
  bool active = false;
  double pidInput = 0, setpoint = 0;
  unsigned long onTime = 0, offTime = 0;

  void notifyError(String error);
  bool isSafe(float temp);
  void refreshPID();
  void simpleHysteresis();
  void refreshMQTT();

 public:
  explicit pidController(board* b);
  ~pidController() override;
  void loop() override;
  void shutdown() override;

  bool getActive() { return active; }
  double getInput() { return pidInput; }
  double getSetpoint() { return setpoint; }
};
