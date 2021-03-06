#pragma once

#include <Arduino.h>

class board;

const int PID_EVALUATE_INTERVAL = 1000;
const int MEAN_INPUT_POINTS = 30;

class controller {
 private:
  board* b = nullptr;
  unsigned long lastLoop = 0, lastCheck = 0;
  unsigned long lastErrorNotify = 0;
  bool active = false;
  float pidInput = 0, setpoint = 0;
  float meanInputPoints[MEAN_INPUT_POINTS]{0};
  int meanInputIdx{0};
  unsigned long onTime = 0, offTime = 0;

  void notifyError(String error);
  bool isSafe(float temp);
  void refreshPID();
  void simpleHysteresis();
  void refreshMQTT();

 public:
  explicit controller(board* b);
  ~controller();
  void loop();
  void shutdown();

  bool getActive() { return active; }
  double getInput() { return pidInput; }
  double getSetpoint() { return setpoint; }
};
