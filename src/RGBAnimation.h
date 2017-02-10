#pragma once

#include "Particle.h"

#if defined(_PARTICLE_BUILD_IDE_)
#  include "RGBmatrixPanel/RGBmatrixPanel.h"
#else
#  include "RGBmatrixPanel.h"
#endif

#undef swap // since RGBmatrixPanel uses Adafruit_mfGFX which #defines swap() which is used in <vector>
#include <vector>
#include "RGBAnimationPath.h"
#include "RGBUtility.h"

class RGBAnimationClass
{
public: 
  enum CAPABILITIES 
  {
    FIXED = 0,
    CAN_MOVE = 1,
    CAN_RESIZE = 2, 
    CAN_MOVE_RESIZE = CAN_MOVE | CAN_RESIZE,
    CAN_ALL = 0xFFFFFFFF
  };
  
protected:
  RGBmatrixPanel & _d;
  POINT            _pos;
  POINT            _size;
  int16_t          _frames;
  int32_t          _backColor;
  int16_t          _curFrame;
  std::vector<RGBAnimationPathClass*> _paths;

  virtual void draw();
  virtual void cls();
  void print3x6(int16_t x, int16_t y, const char* text, uint16_t color, int32_t backColor = -1);

public:
  RGBAnimationClass(RGBmatrixPanel & display);
  RGBAnimationClass(RGBmatrixPanel & display, int16_t x, int16_t y, int16_t w, int16_t h, int16_t frames = -1, int32_t backColor = 0);
  virtual ~RGBAnimationClass() { };

  virtual CAPABILITIES getCaps() { return CAN_ALL; }

  // -1 draws current frame and increments frame counter (returns next frame number) 
  virtual int16_t drawFrame(int16_t frame = -1);  
  
  // moves origin of animation to absolute position
  virtual POINT moveTo(int16_t x, int16_t y, bool clearFirst = true);     
  
  // moves origin of animation relative to its curren position
  virtual POINT move(int16_t dx, int16_t dy, bool clearFirst = true);     

  // set a new size
  virtual POINT setSize(uint16_t w, uint16_t h, bool clearFirst = true);
  
  // shrinks/grows by this amount of pixels
  virtual POINT resize(int16_t dw, int16_t dh, bool clearFirst = true);     

  POINT getPosition() { return _pos; };
  POINT getSize() { return _size; };
  RGBmatrixPanel* getDisplay() { return &_d; }
  
  RGBAnimationPathClass* add(RGBAnimationPathClass* path); // add an animation path

private:
  static const uint8_t  font3x6[][6];
};

