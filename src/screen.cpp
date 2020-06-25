#include "screen.h"

#include "common.h"
#include "main.h"

const unsigned long refreshInterval = 1600;

#define SCREEN_WIDTH 128  // display width, in pixels
#define SCREEN_HEIGHT 64  // display height, in pixels

float floatMap(float x,
               float in_min,
               float in_max,
               float out_min,
               float out_max) {
  const float dividend = out_max - out_min;
  const float divisor = in_max - in_min;
  const float delta = x - in_min;

  return (delta * dividend + (divisor / 2)) / divisor + out_min;
}

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

void screen::graph() {
  auto temp = this->b->water.readTemperature();
  datapoints[lastIndex] = temp;
  lastIndex = (lastIndex + 1) % screen::graphPoints;

  float max = -9999.0, min = 9999.0;
  for (auto i = 0; i < screen::graphPoints; i++) {
    if (datapoints[i] == 0) {
      break;
    }
    if (datapoints[i] > max) {
      max = datapoints[i];
    }
    if (datapoints[i] < min) {
      min = datapoints[i];
    }
  }

  if (min == max) {
    min--;
    max++;
  }

  int16_t prevx = -1, prevy = -1;
  for (auto i = lastIndex; i < lastIndex + screen::graphPoints; i++) {
    auto idx = i % screen::graphPoints;
    auto datapoint = datapoints[idx];
    if (datapoint == 0.0) {
      continue;
    }

    int16_t x = ((i - lastIndex) + 1) * screen::graphSpacing;
    int16_t y = floatMap(datapoint, min, max,
                         screen::graphYStart + screen::graphHeight - 1,
                         screen::graphYStart);

    display.drawPixel(x, y, WHITE);
    if (prevx != -1 && prevy != -1) {
      display.drawLine(x, y, prevx, prevy, WHITE);
    }
    prevx = x;
    prevy = y;
  }

  display.setTextSize(1);
  display.setCursor(0, screen::graphYStart);
  display.printf("%.2f\n", max);
  display.setCursor(0, screen::graphYStart + screen::graphHeight - 7);
  display.printf("%.2f\n", min);
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
  display.setTextSize(2);
  display.printf("Temp %.2f\n", this->b->water.readTemperature());
  graph();
  display.display();
}
