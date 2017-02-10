#pragma once

#include "RGBAnimation.h"

class RGBPlasmaClass : public RGBAnimationClass
{
public:
  RGBPlasmaClass(RGBmatrixPanel & display) : RGBAnimationClass(display) { };
  
  virtual CAPABILITIES getCaps() { return FIXED; }

private:
  virtual void draw();
  
  float    _angle1    = 0.0;
  float    _angle2    = 0.0;
  float    _angle3    = 0.0;
  float    _angle4    = 0.0;
  long     _hueShift  = 0;

  static const int8_t sinetab[];
  static const float radius1;
  static const float radius2;
  static const float radius3;
  static const float radius4;
  static const float centerx1;
  static const float centerx2;
  static const float centerx3;
  static const float centerx4;
  static const float centery1;
  static const float centery2;
  static const float centery3;
  static const float centery4;
};
