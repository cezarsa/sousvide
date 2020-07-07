#pragma once

#include <Adafruit_SSD1306.h>

class board;

class screen {
 private:
  static const int graphWidth = 128, graphHeight = 48, graphSpacing = 4,
                   graphYStart = 16,
                   graphPoints = screen::graphWidth / screen::graphSpacing;

  Adafruit_SSD1306 display;
  board* b = nullptr;
  bool initialized = false;
  unsigned long lastRefresh = 0, lastBlink = 0;
  bool blinkState = true;
  unsigned long lastGraphRefresh = 0;
  float datapoints[screen::graphPoints] = {0};
  unsigned int lastIndex = 0;

  float lastSetpoint = -999;

  void graph(unsigned long now);
  bool initialize();
  bool chooseSetpoint();

 public:
  screen();
  ~screen();

  void bind(board* b);
  void loop();
};