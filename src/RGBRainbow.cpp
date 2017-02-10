#include "RGBRainbow.h"

void RGBRainbowClass::draw()
{
  switch(_ori)      
  {
    case TOPLEFT_BOTTOMRIGHT:
    {
      POINT p[2] = { POINT(_pos.x, _pos.y), POINT(_pos.x, _pos.y) }; // points for lines
      POINT d[2] = { POINT(+1,0), POINT(0,+1) };                     // diretiction of movement
      for(int i = 0; i < _size.x+_size.y-1; i++)
      {
        int hue = (_speed > 0 ? 1 : -1) * (_hue + i * _step);
        _d.drawLine(p[0].x, p[0].y, p[1].x, p[1].y, _d.ColorHSV(hue, 127, 255, _gamma));

        p[0] += d[0];
        p[1] += d[1];

        if (p[0].x >= _pos.x+_size.x-1)
        {
          d[0].x =  0;
          d[0].y = +1;
        }
        if (p[1].y >= _pos.y+_size.y-1)
        {
          d[1].x = +1;
          d[1].y =  0;
        }
      } 
      break;
    }
    case TOPRIGHT_BOTTOMLEFT:
    {
      POINT p[2] = { POINT(_pos.x+_size.x-1, _pos.y), POINT(_pos.x+_size.x-1, _pos.y) }; // points for lines
      POINT d[2] = { POINT(-1,0) , POINT(0,+1) };                                        // diretiction of movement
      
      for(int i = 0; i < _size.x+_size.y-1; i++)
      {
        int hue = (_speed > 0 ? 1 : -1) * (_hue + i * _step);
        _d.drawLine(p[0].x, p[0].y, p[1].x, p[1].y, _d.ColorHSV(hue, 127, 255, _gamma));

        p[0] += d[0];
        p[1] += d[1];

        if (p[0].x <= _pos.x)
        {
          d[0].x =  0;
          d[0].y = +1;
        }
        if (p[1].y >= _pos.y+_size.y-1)
        {
          d[1].x = -1;
          d[1].y =  0;
        }
      }
      break;
    }
    case VERTICAL:
    {
      for(int i = 0; i < _size.y; i++)
        _d.drawLine(_pos.x+i, _pos.y, _pos.x+i, _pos.y+_size.y-1, _d.ColorHSV(_hue+i*_step, 127, 255, _gamma));
      break;
    }
    case HORIZONTAL:
    {
      for(int i = 0; i < _size.x; i++)
        _d.drawLine(_pos.x, _pos.y+i, _pos.x+_size.x-1, _pos.y+i, _d.ColorHSV(_hue+i*_step, 127, 255, _gamma));
      break;
    }
    case CENTER_RAY:
    {
      int t = 0;
      // quick and dirty - I guess I could unify these into one for (2*(_size.x+_size.y-1)) but can't be bothered at the mo'
      for(int i = 0; i < _size.x; i++, t++)
        _d.drawLine(_pos.x+_size.x/2, _pos.y+_size.y/2, _pos.x+i, _pos.y, _d.ColorHSV(_hue+t*_step, 127, 255, _gamma));
      for(int i = 0; i < _size.y; i++, t++)
        _d.drawLine(_pos.x+_size.x/2, _pos.y+_size.y/2, _pos.x+_size.x-1, _pos.y+i, _d.ColorHSV(_hue+t*_step, 127, 255, _gamma));
      for(int i = 0; i < _size.x; i++, t++)
        _d.drawLine(_pos.x+_size.x/2, _pos.y+_size.y/2, _pos.x+_size.x-1-i, _pos.y+_size.y-1, _d.ColorHSV(_hue+t*_step, 127, 255, _gamma));
      for(int i = 0; i < _size.y; i++, t++)
        _d.drawLine(_pos.x+_size.x/2, _pos.y+_size.y/2, _pos.x, _pos.y+_size.y-1-i, _d.ColorHSV(_hue+t*_step, 127, 255, _gamma));
      break;
    }
    case CENTER_BOX:
    {
      float x,y,w,h,err;
      x   = _pos.x;
      y   = _pos.y;
      w   = _size.x;
      h   = _size.y;
      err = 0;
      
      for(int i=0; w > 1 || h > 1 || (w == 1 && h == 1); i++)
      {
        _d.drawRect(x, y, w, h, _d.ColorHSV(_hue+i*_step, 127, 255, _gamma));

        if (_size.x >= _size.y)
        {
          x += 1;
          w -= 2;
          if ((err += (float)_size.y/(float)_size.x) && h >= 2)
          {
            y += 1;
            h -= 2;
            err--;
          }
        }
        else
        {
          y += 1;
          h -= 2;
          if ((err += (float)_size.x/(float)_size.y) && w >= 2)
          {
            x += 1;
            w -= 2;
            err--;
          }
        }
      }
      break;
    }
    default:
      _ori = HORIZONTAL;
      break;
  }

  _hue += _speed;
  _hue %= 1536;
}
