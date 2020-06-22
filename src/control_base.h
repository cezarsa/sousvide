#pragma once

class board;

class controller {
 public:
  virtual ~controller(){};
  virtual void setup(board* b) = 0;
  virtual void loop() = 0;
};
