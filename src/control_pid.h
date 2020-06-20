#ifndef _SOUSVIDE_CONTROL_PID_H_
#define _SOUSVIDE_CONTROL_PID_H_

#include <Arduino.h>
#include <PID_v1.h>

#include "control_base.h"

const int WINDOW_SIZE = 10000;

class pidController : public controller {
 private:
  PID* pid = nullptr;
  board* b = nullptr;
  unsigned long lastLoop = 0;
  unsigned long lastErrorNotify = 0;
  unsigned long windowStartTime = 0;
  bool active = false;
  double kp = 2, ki = 5, kd = 1;
  double pidInput = 0, pidOutput = 0, setpoint = 0;

  void notifyError(String error);
  bool isSafe(float temp);
  void refreshPID();
  void refreshMQTT();

 public:
  pidController();
  ~pidController();
  void setup(board* b) override;
  void loop() override;
};

#endif
