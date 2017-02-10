#include "RGBNormal.h"

RGBNormalClass::RGBNormalClass(RGBmatrixPanel & display, int16_t x, int16_t y, CLOCK_SIZE size)
: RGBAnimationClass(display) 
{ 
  switch(size)    
  {
    case SMALL:
      _size.x = 16;
      _size.y = 8;
      _fontSize = 0;
      break;
    case LARGE:
      _size.x = 64;
      _size.y = 32;
      _fontSize = 2;
      break;
    case MEDIUM:
    default:
      _size.x = 32;
      _size.y = 16;
      _fontSize = 1;
      break;
  }
};

void RGBNormalClass::draw() 
{
  char time[6];
  strcpy(time, Time.format("%H:%M"));
  
  cls();
  if (_fontSize > 0)
  {
    _d.setTextSize(_fontSize);
    _d.setTextColor(_d.Color444(0,0,3));
    _d.setCursor(_pos.x + 2*_fontSize, _pos.y + 5*_fontSize);
    _d.print(time);
    _d.setCursor(_pos.x + 1*_fontSize, _pos.y + 4*_fontSize);
    _d.setTextColor(_d.Color444(15,15,15));
    _d.print(time);
    _d.drawRect(_pos.x, _pos.y, _size.x, _size.y, _d.Color333(1,1,1));
  }
  else
  {
    print3x6(_pos.x+1, _pos.y+2, time, _d.Color444(0,0,3));
    print3x6(_pos.x, _pos.y + 1, time, _d.Color444(15,15,15));
  }
}

