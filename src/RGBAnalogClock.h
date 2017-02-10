#pragma once

#include "Particle.h"
#include "RGBAnimation.h"

class RGBAnalogClockClass : public RGBAnimationClass
{
public:
  RGBAnalogClockClass(RGBmatrixPanel & display, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t colorFace, uint16_t colorHour, uint16_t colorMinute, uint32_t colorSecond = -1, uint32_t backColor = -1)
  : RGBAnimationClass(display, x, y, w, h, -1, backColor), _cFace(colorFace), _cHH(colorHour), _cMM(colorMinute), _cSS(colorSecond) { };
  //virtual ~RGBAnalogClockClass() { };

  virtual CAPABILITIES getCaps() { return CAN_MOVE_RESIZE; }

private: 
  virtual void draw();

  uint16_t _cFace;
  uint16_t _cHH;
  uint16_t _cMM;
  uint16_t _cSS;

  POINT pointOnEllipse(int16_t width, int16_t height, uint16_t angle);
  void drawEllipse(int16_t cX, int16_t cY, int16_t width, int16_t height, uint16_t color);
};