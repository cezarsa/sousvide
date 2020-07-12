#include "controller.h"

#include "common.h"
#include "main.h"

controller::~controller() {}

controller::controller(board* b) : b(b) {
  b->mqttConn->listen("control/setpoint", [this](String value) {
    setpoint = atof(value.c_str());
    refreshPID();
  });
  b->mqttConn->listen("control/state", [this](String value) {
    active = value.equalsIgnoreCase("true") || value.equalsIgnoreCase("1");
    refreshPID();
  });
}

void controller::refreshPID() {
  if (setpoint == 0.0f || !active) {
    b->heater.off();
    delay(1000);
    b->pump.off();
    lastLoop = 0;
    lastCheck = 0;
    onTime = 0;
    offTime = 0;
  }
  refreshMQTT();
}

void controller::refreshMQTT() {
  b->mqttConn->publish("control/setpoint", String(setpoint));
  b->mqttConn->publish("control/input", String(pidInput));
  b->mqttConn->publish("control/state", String(active));
}

void controller::loop() {
  unsigned long now = millis();
  if (lastLoop != 0 && now - lastLoop < PID_EVALUATE_INTERVAL) {
    return;
  }
  lastLoop = now;

  pidInput = b->water.readTemperature();
  if (!isSafe(pidInput)) {
    return;
  }

  meanInputPoints[meanInputIdx] = pidInput;
  meanInputIdx = (meanInputIdx + 1) % MEAN_INPUT_POINTS;

  if (setpoint == 0.0f || !active) {
    refreshMQTT();
    return;
  }

  if (!b->pump.getState()) {
    b->pump.on();
    delay(1000);
  }
  simpleHysteresis();
  refreshMQTT();
}

void controller::simpleHysteresis() {
  unsigned long now = millis();

  if (lastCheck != 0) {
    if (b->heater.getState()) {
      onTime += now - lastCheck;
      offTime = 0;
    } else {
      offTime += now - lastCheck;
      onTime = 0;
    }
  }
  lastCheck = now;

  int validPoints = 0;
  float meanInput = 0.f;
  for (auto v : meanInputPoints) {
    if (v == 0) {
      break;
    }
    validPoints++;
    meanInput += v;
  }
  meanInput /= validPoints;

  if (meanInput < (setpoint - 2)) {
    b->heater.on();
    return;
  }

  if (pidInput < (setpoint - 0.4f)) {
    if (onTime >= 4000) {
      b->heater.off();
    } else if (offTime >= 5000) {
      b->heater.on();
    }
    return;
  }

  if (meanInput < setpoint) {
    if (onTime >= 3000) {
      b->heater.off();
    } else if (offTime >= 10000) {
      b->heater.on();
    }
    return;
  }

  b->heater.off();
}

bool controller::isSafe(float temp) {
  if (temp <= 0) {
    b->heater.off();
    delay(1000);
    b->pump.off();
    notifyError(
        String(
            "invalid temperature, assuming dead sensor and shutting down: ") +
        temp);
    return false;
  }

  if (temp > 90) {
    b->heater.off();
    delay(1000);
    b->pump.off();
    notifyError(String("temperature too high, shutting down for safety: ") +
                temp);
    return false;
  }

  return true;
}

void controller::notifyError(String error) {
  logger.printf("[controller] %s\n", error.c_str());

  unsigned long now = millis();
  if (lastErrorNotify != 0 && now - lastErrorNotify < 30000) {
    return;
  }
  lastErrorNotify = now;
  b->mqttConn->publish("control/error", error);
}

void controller::shutdown() {
  setpoint = 0.0;
  active = false;
  refreshPID();
}
