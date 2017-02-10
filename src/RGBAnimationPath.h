#pragma once

#include "RGBUtility.h"
//#include "RGBAnimation.h"
class RGBAnimationClass;

class RGBAnimationPathClass
{
public:
  enum STATE : int {
    STOPPED = 0,
    RUNNING,
  };
  
  RGBAnimationPathClass() : _owner(0), _state(STOPPED) { };
  RGBAnimationPathClass(RGBAnimationClass* owner) : _owner(owner), _state(STOPPED) { };
  virtual ~RGBAnimationPathClass() { };
  virtual void animate() { Serial.println("base!"); };
  virtual int getState() { Serial.println("base!") ; return (int)_state; };
  
protected:
  RGBAnimationClass* _owner;
  STATE              _state;

friend class RGBAnimationClass;
};
