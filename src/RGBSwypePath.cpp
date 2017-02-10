#include "RGBSwypePath.h"
#include "RGBAnimation.h"

void RGBSwypePathClass::start(int acceleration, int frameRate)
{
  _origin = _owner->getPosition();
  _frame = 0;
  _frameRate = frameRate;
  _speed = 0;
  _accel = acceleration;
  _state = SWYPEOUT;
}

void RGBSwypePathClass::animate()
{
  if (_state == STOPPED) return;    
    
  POINT _size = _owner->getSize();
  POINT _disp = POINT(_owner->getDisplay()->width(), _owner->getDisplay()->height());
  
  if (!(_frame++ % _frameRate))
  {
    _speed += _accel;

    switch(_dir)
    {
      case LEFT:
        if (_owner->move(-_speed, 0, true).x <= 0 - _size.x)
        {
          _owner->move(+(_disp.x + _size.x), 0);
          if (_state != SWYPEIN)
            _speed = (_disp.x - _origin.x) / 3;
          else
            _speed++;
          _accel = -1;
          _state = SWYPEIN;
        }
        break;
      case RIGHT:
        if (_owner->move(+_speed, 0, true).x >= _disp.x)
        {
          _owner->move(-(_disp.x + _size.x), 0);
          if (_state != SWYPEIN)
            _speed = (_size.x + _origin.x) / 3;
          else
            _speed++;
          _accel = -1;
          _state = SWYPEIN;
        }
        break;
      case UP:
        if (_owner->move(0, -_speed, true).y <= 0 - _size.y)
        {
          _owner->move(0, +(_disp.y + _size.y));
          if (_state != SWYPEIN)
            _speed = (_disp.y - _origin.y) / 3;
          else
            _speed++;
          _accel = -1;
          _state = SWYPEIN;
        }
        break;
      case DOWN:
        if (_owner->move(0, +_speed, true).y >= _disp.y)
        {
          _owner->move(0, -(_disp.y + _size.y));
          if (_state != SWYPEIN)
            _speed = (_size.y + _origin.y) / 3;
          else
            _speed++;
          _accel = -1;
          _state = SWYPEIN;
        }
        break;
      default:
        _state = STOPPED;
        break;
    }

    // make sure to return to original position if runs too long
    if (_speed < -2 || _frame > 1000)
    {
      _owner->moveTo(_origin.x, _origin.y);
      _frame = 0;
      _accel = 0;
      _speed = 0;
      _state = STOPPED;
    }
  }
}
