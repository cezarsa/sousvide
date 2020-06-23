#include "screen.h"

#include "common.h"
#include "main.h"

const unsigned long refreshInterval = 1500;

#define SCREEN_WIDTH 128  // display width, in pixels
#define SCREEN_HEIGHT 64  // display height, in pixels

screen::screen() : display(SCREEN_WIDTH, SCREEN_HEIGHT) {}

screen::~screen() {}

void screen::bind(board* b) {
  this->b = b;
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0)) {
    logger.println(F("[screen] SSD1306 allocation failed"));
  }
}

bool screen::initialize() {
  if (initialized) {
    return true;
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    logger.println(F("[screen] SSD1306 allocation failed"));
    return false;
  }

  logger.println(F("[screen] SSD1306 initialized"));

  initialized = true;
  display.setTextSize(2);
  display.setTextColor(WHITE);

  return true;
}

void screen::loop() {
  unsigned long now = millis();
  if (lastRefresh != 0 && now - lastRefresh < refreshInterval) {
    return;
  }
  lastRefresh = now;

  if (!initialize()) {
    return;
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.printf("Temp %.2f\n", this->b->water.readTemperature());
  display.display();
}
