#pragma once

#include "RGBAnimationPath.h"
#include "RGBAnimation.h"

class RGBSwypePathClass : public RGBAnimationPathClass
{
public:
  enum STATE : int {
    STOPPED = 0,
    SWYPEOUT,
    SWYPEIN,
  };

  enum DIRECTION 
  {
    RIGHT,
    LEFT,
    UP,
    DOWN,
  }; 

  RGBSwypePathClass(DIRECTION direction) : RGBAnimationPathClass(), _dir(direction), _state(STOPPED) { };
  RGBSwypePathClass(RGBAnimationClass* owner, DIRECTION direction) : RGBAnimationPathClass(owner), _dir(direction), _state(STOPPED) { };
  void start(int acceleration = 1, int frameRate = 1);
  virtual void animate();
  virtual int  getState() { return (int)_state; };

private:
  STATE     _state;
  DIRECTION _dir;
  int       _accel;
  int       _frame;      // animation frame count (for current path run)
  int       _frameRate;  // animation frames per path steps
  int       _speed;
  POINT     _origin;
};
