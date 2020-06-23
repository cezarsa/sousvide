#pragma once

#include <Adafruit_SSD1306.h>

class board;

class screen {
 private:
  Adafruit_SSD1306 display;
  board* b;
  bool initialized = false;
  unsigned long lastRefresh = 0;

 public:
  screen();
  ~screen();

  void bind(board* b);
  void loop();
  bool initialize();
};