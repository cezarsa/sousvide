#ifndef _SOUSVIDE_CONTROL_BASE_H_
#define _SOUSVIDE_CONTROL_BASE_H_

class board;

class controller {
 public:
  virtual ~controller(){};
  virtual void setup(board* b) = 0;
  virtual void loop() = 0;
};

#endif
