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
  unsigned long lastRefresh = 0;
  float datapoints[screen::graphPoints] = {0};
  unsigned int lastIndex = 0;

 public:
  screen();
  ~screen();

  void bind(board* b);
  void loop();
  void graph();
  bool initialize();
};