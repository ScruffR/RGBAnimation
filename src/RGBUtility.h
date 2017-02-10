#pragma once

#include "Particle.h"

struct POINT
{
  POINT() : x(0), y(0) { };
  POINT(int16_t X, int16_t Y) : x(X), y(Y) { };
  
  int16_t x;
  int16_t y;
  
  virtual POINT & operator=(const POINT & p) { this->x = p.x; this->y = p.y; return *this; }; 
  virtual POINT & operator+=(const POINT & p) { this->x += p.x; this->y += p.y; return *this; };
  virtual bool    operator==(const POINT & p) { return (this->x == p.x && this->y == p.y); }; 
  virtual bool    operator!=(const POINT & p) { return (this->x != p.x || this->y != p.y); }; 
};

