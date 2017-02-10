#pragma once

#include "RGBAnimation.h"

class RGBNormalClass : public RGBAnimationClass
{
public:
  enum CLOCK_SIZE {
      SMALL,
      MEDIUM,
      LARGE,
  };
  
  RGBNormalClass(RGBmatrixPanel & display, int16_t x, int16_t y, CLOCK_SIZE size = MEDIUM);

  virtual CAPABILITIES getCaps() { return CAN_MOVE; }

private:
  virtual void draw();

  uint8_t _fontSize;
};