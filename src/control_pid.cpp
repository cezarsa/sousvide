#include "control_pid.h"

#include "common.h"
#include "main.h"

pidController::~pidController() {}

pidController::pidController(board* b) : b(b) {
  b->mqttConn->listen("control/kp", [this](String value) {
    kp = atof(value.c_str());
    refreshPID();
  });
  b->mqttConn->listen("control/ki", [this](String value) {
    ki = atof(value.c_str());
    refreshPID();
  });
  b->mqttConn->listen("control/kd", [this](String value) {
    kd = atof(value.c_str());
    refreshPID();
  });
  b->mqttConn->listen("control/setpoint", [this](String value) {
    setpoint = atof(value.c_str());
    refreshPID();
  });
  b->mqttConn->listen("control/state", [this](String value) {
    active = value.equalsIgnoreCase("true") || value.equalsIgnoreCase("1");
    refreshPID();
  });
}

void pidController::refreshPID() {
  if (setpoint == 0.0 || !active) {
    b->heater.off();
    b->pump.off();
    pidOutput = 0;
    if (pid) {
      delete pid;
      pid = nullptr;
    }
    refreshMQTT();
    return;
  }

  b->pump.on();

  if (pid) {
    pid->SetTunings(kp, ki, kd);
    refreshMQTT();
    return;
  }

  pid = new PID(&pidInput, &pidOutput, &setpoint, kp, ki, kd, DIRECT);
  pid->SetOutputLimits(0, WINDOW_SIZE);
  pid->SetMode(AUTOMATIC);
  windowStartTime = millis();
  refreshMQTT();
}

void pidController::refreshMQTT() {
  b->mqttConn->publish("control/kp", String(kp));
  b->mqttConn->publish("control/ki", String(ki));
  b->mqttConn->publish("control/kd", String(kd));
  b->mqttConn->publish("control/setpoint", String(setpoint));
  b->mqttConn->publish("control/input", String(pidInput));
  b->mqttConn->publish("control/output", String(pidOutput));
  b->mqttConn->publish("control/state", String(active));
}

void pidController::loop() {
  unsigned long now = millis();
  if (now - lastLoop < 1000) {
    return;
  }
  lastLoop = now;

  float temp = b->water.readTemperature();
  pidInput = double(temp);
  if (!isSafe(temp)) {
    return;
  }
  if (!pid) {
    refreshMQTT();
    return;
  }

  pid->Compute();

  if (millis() - windowStartTime > WINDOW_SIZE) {
    windowStartTime += WINDOW_SIZE;
  }
  if (pidOutput < millis() - windowStartTime) {
    b->heater.off();
  } else {
    b->heater.on();
  }
  refreshMQTT();
}

bool pidController::isSafe(float temp) {
  if (temp <= 0) {
    b->heater.off();
    b->pump.off();
    notifyError(
        String(
            "invalid temperature, assuming dead sensor and shutting down: ") +
        temp);
    return false;
  }

  if (temp > 90) {
    b->heater.off();
    b->pump.off();
    notifyError(String("temperature too high, shutting down for safety: ") +
                temp);
    return false;
  }

  return true;
}

void pidController::notifyError(String error) {
  logger.printf("[pid controller] %s\n", error.c_str());

  unsigned long now = millis();
  if (now - lastErrorNotify < 30000) {
    return;
  }
  lastErrorNotify = now;
  b->mqttConn->publish("control/error", error);
}

void pidController::shutdown() {
  setpoint = 0.0;
  active = false;
  refreshPID();
}
