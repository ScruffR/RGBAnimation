#pragma once

#include "RGBAnimation.h"

class RGBPongClass : public RGBAnimationClass
{
public:
  RGBPongClass(RGBmatrixPanel & display, int16_t x, int16_t y, int32_t backColor = -1) 
  : RGBAnimationClass(display, x, y, 32, 16, -1, backColor) { };
  
  virtual CAPABILITIES getCaps() { return CAN_MOVE; }

private:
  virtual void draw();

  byte pong_get_ball_endpoint(float tempballpos_x, float  tempballpos_y, float  tempballvel_x, float tempballvel_y);

  int16_t bat1_y        = 5;   //bat starting y positions
  int16_t bat2_y        = 5;
  int16_t bat1_target_y = 5;   //bat targets for bats to move to
  int16_t bat2_target_y = 5;
  byte bat1_update      = 1;   //flags - set to update bat position
  byte bat2_update      = 1;
  byte restart          = 1;   //game restart
  byte bat1miss, bat2miss;     //flags set on the minute or hour that trigger the bats to miss the ball, thus upping the score to match the time. 
  float ballpos_x, ballpos_y;
  float ballvel_x, ballvel_y;

  static const int16_t BAT_HEIGHT;
  static const int16_t BAT1_X; // collision detection
  static const int16_t BAT2_X;
};
