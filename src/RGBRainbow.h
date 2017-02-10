#pragma once

#include "RGBAnimation.h"

class RGBRainbowClass : public RGBAnimationClass
{
public:
  enum ORIENTATION 
  {
    HORIZONTAL,
    VERTICAL,
    TOPLEFT_BOTTOMRIGHT,
    TOPRIGHT_BOTTOMLEFT,
    CENTER_RAY,
    CENTER_BOX,
  }; 
  
  RGBRainbowClass(RGBmatrixPanel & display, int16_t x, int16_t y, int16_t w, int16_t h, int step = 1, int speed = 1, bool gamma = true, ORIENTATION orientation = HORIZONTAL)
  : RGBAnimationClass(display, x, y, w, h, -1, -1), _step(step), _speed(speed), _hue(0), _gamma(gamma), _ori(orientation) { };
  RGBRainbowClass(RGBmatrixPanel & display, int step = 1, int speed = 1, ORIENTATION orientation = HORIZONTAL)
  : RGBAnimationClass(display), _step(step), _speed(speed), _hue(0), _gamma(true), _ori(orientation) { };
  
  virtual CAPABILITIES getCaps() { return CAN_MOVE_RESIZE; }

private:
  virtual void draw();
  
  int         _step;
  int         _speed;
  int         _hue;
  bool        _gamma;
  ORIENTATION _ori;
};
