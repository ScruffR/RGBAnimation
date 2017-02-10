#include "RGBAnalogClock.h"
#include <math.h>

void RGBAnalogClockClass::draw()
{
  int cX = _pos.x + _size.x/2;
  int cY = _pos.y + _size.y/2;
  int w  = _size.x/2;
  int h  = _size.y/2;
  POINT pt;
  
  cls();
  
  drawEllipse(cX, cY, w, h, _cFace);
  if (_cSS != _backColor)
  {
    pt = pointOnEllipse(w-1, h-1, Time.second()*6 +270);
    //_d.drawLine(cX, cY, cX + pt.x, cY + pt.y, _cSS);
    _d.drawPixel(cX + pt.x, cY + pt.y, _cSS);
  }
  if (_cMM != _backColor)
  {
    pt = pointOnEllipse(w-2, h-2, Time.minute()*6 +270);
    _d.drawLine(cX, cY, cX + pt.x, cY + pt.y, _cMM);
  }
  if (_cHH != _backColor)
  {
    pt = pointOnEllipse(w/2, h/2, Time.hour()*30 + Time.minute()*0.5 +270);
   _d.drawLine(cX, cY, cX + pt.x, cY + pt.y, _cHH);
  }
  //print3x6(_pos.x+_size.x-8, _pos.y+_size.y/2 - 3, Time.format("%S"), _cFace);
}

void RGBAnalogClockClass::drawEllipse(int16_t cX, int16_t cY, int16_t width, int16_t height, uint16_t color)
{
    int a2  = width * width;
    int b2  = height * height;
    int fa2 = 4 * a2;
    int fb2 = 4 * b2;
    int dX  = 1 - cX%2; // if center point is an even coordinate
    int dY  = 1 - cY%2; // add a one pixle slice, to counteract trunaction of .5 results
    int x, y, sigma;

    /* first half */
    for (x = 0, y = height, sigma = 2*b2+a2*(1-2*height); b2*x <= a2*y; x++)
    {
        _d.drawPixel(cX + x -dX, cY + y -dY, color);
        _d.drawPixel(cX - x    , cY + y -dY, color);
        _d.drawPixel(cX + x -dX, cY - y    , color);
        _d.drawPixel(cX - x    , cY - y    , color);
        if (sigma >= 0)
        {
            sigma += fa2 * (1 - y);
            y--;
        }
        sigma += b2 * ((4 * x) + 6);
    }

    /* second half */
    for (x = width, y = 0, sigma = 2*a2+b2*(1-2*width); a2*y <= b2*x; y++)
    {
        _d.drawPixel(cX + x -dX, cY + y -dY, color);
        _d.drawPixel(cX - x    , cY + y -dY, color);
        _d.drawPixel(cX + x -dX, cY - y    , color);
        _d.drawPixel(cX - x    , cY - y    , color);
        if (sigma >= 0)
        {
            sigma += fb2 * (1 - x);
            x--;
        }
        sigma += a2 * ((4 * y) + 6);
    }
}

POINT RGBAnalogClockClass::pointOnEllipse(int16_t width, int16_t height, uint16_t angle)
{
  double a     = width;
  double b     = height;
  double theta = angle * M_PI / 180.0;
  double st    = sin(theta);
  double ct    = cos(theta);
  double q     = sqrt(b*b*ct*ct + a*a*st*st);
  
  return POINT((a*b*ct)/q, (a*b*st)/q);
}
