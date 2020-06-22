#pragma once

class board;

class controller {
 public:
  virtual ~controller(){};
  virtual void loop() = 0;
  virtual void shutdown() = 0;
};
