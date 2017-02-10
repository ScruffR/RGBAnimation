#include "RGBPong.h"

void RGBPongClass::draw() 
{
	cls();
	//draw pitch centre line

    print3x6(_pos.x + _size.x / 2 - 7, _pos.y + 1, Time.format("%H %M"), _d.Color333(7,7,0), 0);
	for (byte i = Time.second() % 2; i < _size.y; i+=2) 
	{
	  _d.drawPixel(_pos.x + _size.x / 2, _pos.y + i, _d.Color333(0,4,0));
	}
	_d.drawRect(_pos.x, _pos.y, _size.x, _size.y, _d.Color333(2,2,2));
    
	//if restart flag is 1, setup a new game
	if (restart) 
	{
	  ballpos_x = _pos.x + _size.x / 2;
	  if (Time.second() == restart || 0 >= Time.second() || Time.second() >= 59)
	  { // wait before restart
	    ballpos_y = _pos.y + _size.y / 2;
        ballvel_x = 0;
        ballvel_y = 0;
	  }
	  else
	  {
        //set ball start pos
	    ballpos_y = random (_pos.y + 4, _pos.y + _size.y - 4);
		//pick "random" ball direction
        ballvel_x = millis() % 2 ? 1 : -1;
		ballvel_y = micros() % 2 ? 0.5 : -0.5;
      
  		//reset game restart flag
		restart = 0;
	  }
      // draw bats in initial positions
	  bat1miss = 0;
      bat2miss = 0;
	}

	//if coming up to the minute: secs = 59 and mins < 59, flag bat 2 (right side) to miss the return so we inc the minutes score
	if (Time.second() == 59 && Time.minute() < 59) {
		bat1miss = 1;
	}
	// if coming up to the hour: secs = 59  and mins = 59, flag bat 1 (left side) to miss the return, so we inc the hours score.
	if (Time.second() == 59 && Time.minute() == 59) {
		bat2miss = 1;
	}

	//AI - we run 2 sets of 'AI' for each bat to work out where to go to hit the ball back
	//very basic AI...
	// For each bat, First just tell the bat to move to the height of the ball when we get to a random location.
	//for bat1
	if (ballpos_x == random(_pos.x + _size.x/2 + 2, _pos.x + _size.x)) {
		bat1_target_y = ballpos_y;
	}
	//for bat2
	if (ballpos_x == random(_pos.x, _pos.x + _size.x / 2)) {
		bat2_target_y = ballpos_y;
	}

	//when the ball is closer to the left bat, run the ball maths to find out where the ball will land
	if (ballpos_x == _pos.x + _size.x / 2 - 1 && ballvel_x < 0) {
		byte end_ball_y = pong_get_ball_endpoint(ballpos_x, ballpos_y, ballvel_x, ballvel_y);

		//if the miss flag is set, then the bat needs to miss the ball when it gets to end_ball_y
		if (bat1miss == 1) {
			bat1miss = 0;
			if (end_ball_y > _pos.y + _size.y / 2) {
				bat1_target_y = _pos.y + random(0,3);
			}
			else {
				bat1_target_y = _pos.y + _size.y - random(0,3);
			}
		}
		//if the miss flag isn't set,  set bat target to ball end point with some randomness so its not always hitting top of bat
		else {
			bat1_target_y = constrain(end_ball_y - random(0, BAT_HEIGHT), _pos.y, _pos.y + _size.y - BAT_HEIGHT);
		}
	}

	//right bat AI
	//if positive velocity then predict for right bat - first just match ball height
	//when the ball is closer to the right bat, run the ball maths to find out where it will land
	if (ballpos_x == _pos.x + _size.x / 2 - 1 && ballvel_x > 0) {
		byte end_ball_y = pong_get_ball_endpoint(ballpos_x, ballpos_y, ballvel_x, ballvel_y);

		//if flag set to miss, move bat out way of ball
		if (bat2miss == 1){
			bat2miss = 0;
			//if ball end point above 8 then move bat down, else move it up- so either way it misses
			if (end_ball_y > _pos.y + _size.y / 2) {
				bat2_target_y = _pos.y + random (0,3);
			}
			else {
				bat2_target_y = _pos.y + _size.y - random (0,3);
			}
		}
		else {
			//set bat target to ball end point with some randomness
			bat2_target_y =  constrain(end_ball_y - random(0, BAT_HEIGHT), _pos.y, _pos.y + _size.y - BAT_HEIGHT);
		}
	}

	//move bat 1 towards target
	//if bat y greater than target y move down until hit top bound (don't go any further or bat will move off screen)
	if (bat1_y > bat1_target_y && bat1_y > _pos.y ) {
		bat1_y--;
		bat1_update = 1;
	}

	//if bat y less than target y move up until hit bottom bound
	if (bat1_y < bat1_target_y && bat1_y < _pos.y + _size.y - BAT_HEIGHT) {
		bat1_y++;
		bat1_update = 1;
	}

	//draw bat 1
	if (bat1_update){
		_d.fillRect(_pos.x + BAT1_X-1, bat1_y, 2, BAT_HEIGHT, _d.Color333(0,0,4));
	}

	//move bat 2 towards target (dont go any further or bat will move off screen)
	//if bat y greater than target y move up until hit top bound
	if (bat2_y > bat2_target_y && bat2_y > _pos.y ) {
		bat2_y--;
		bat2_update = 1;
	}

	//if bat y less than target y move up until hit bound
	if (bat2_y < bat2_target_y && bat2_y < _pos.y + _size.y - BAT_HEIGHT) {
		bat2_y++;
		bat2_update = 1;
	}

	//draw bat2
	if (bat2_update){
		_d.fillRect(_pos.x + BAT2_X+1, bat2_y, 2, BAT_HEIGHT, _d.Color333(0,0,4));
	}

	//update the ball position using the velocity
	ballpos_x =  ballpos_x + ballvel_x;
	ballpos_y =  ballpos_y + ballvel_y;

	//check ball collision with top and bottom of screen and reverse the y velocity if either is hit
	if (ballpos_y <= _pos.y ) {
		ballvel_y = ballvel_y * -1;
		ballpos_y = _pos.y; //make sure value goes no less that top bound
	}

	if (ballpos_y >= _pos.y + _size.y - 1) {
		ballvel_y = ballvel_y * -1;
		ballpos_y =  _pos.y + _size.y - 1; //make sure value goes no more than 15
	}

	//check for ball collision with bat1. check ballx is same as batx
	//and also check if bally lies within width of bat i.e. baty to baty + 6. We can use the exp if(a < b && b < c)
	if ((int)ballpos_x == _pos.x + BAT1_X + 1 && (bat1_y <= (int)ballpos_y && (int)ballpos_y <= bat1_y + BAT_HEIGHT-1) ) {

		//random if bat flicks ball to return it - and therefor changes ball velocity
		if(!random(0,3)) { //not true = no flick - just straight rebound and no change to ball y vel
			ballvel_x *= -1;
		}
		else {
			bat1_update = 1;
			byte flick = 0;  //0 = up, 1 = down.

			if (bat1_y > _pos.y+1 || bat1_y < _pos.y + _size.y / 2) {
				flick = random(0,2);   //pick a random dir to flick - up or down
			}

			//if bat 1 or 2 away from top only flick down
			if (bat1_y <= _pos.y + 1) {
				flick = 0;   //move bat down 1 or 2 pixels
			}
			//if bat 1 or 2 away from bottom only flick up
			if (bat1_y >= _pos.y + _size.y / 2){
				flick = 1;  //move bat up 1 or 2 pixels
			}

			switch (flick) {
				//flick up
			  case 0:
				bat1_target_y += random(1,3);
				ballvel_x *= -1;
				if (ballvel_y < 2) {
					ballvel_y += 0.2;
				}
				break;

				//flick down
			  case 1:
				bat1_target_y -= random(1,3);
				ballvel_x *= -1;
				if (ballvel_y > 0.2) {
					ballvel_y -= 0.2;
				}
				break;
			}
		}
	}

	//check for ball collision with bat2. check ballx is same as batx
	//and also check if bally lies within width of bat i.e. baty to baty + 6. We can use the exp if(a < b && b < c)
	if ((int)ballpos_x == _pos.x + BAT2_X && (bat2_y <= (int)ballpos_y && (int)ballpos_y <= bat2_y + BAT_HEIGHT - 1) ) {

		//random if bat flicks ball to return it - and therefor changes ball velocity
		if(!random(0,3)) {
			ballvel_x *= -1;    //not true = no flick - just straight rebound and no change to ball y vel
		}
		else {
			bat1_update = 1;
			byte flick = 0;  //0 = up, 1 = down.

			if (bat2_y > _pos.y + 1 || bat2_y < _pos.y + _size.y / 2) {
				flick = random(0,2);   //pick a random dir to flick - up or down
			}
			//if bat 1 or 2 away from top only flick down
			if (bat2_y <= _pos.y + 1) {
				flick = 0;  //move bat up 1 or 2 pixels
			}
			//if bat 1 or 2 away from bottom only flick up
			if (bat2_y >= _pos.y + _size.y / 2) {
				flick = 1;   //move bat down 1 or 2 pixels
			}

			switch (flick) {
				//flick up
			case 0:
				bat2_target_y += random(1,3);
				ballvel_x *= -1;
				if (ballvel_y < 2) {
					ballvel_y += 0.2;
				}
				break;

				//flick down
			case 1:
				bat2_target_y -= random(1,3);
				ballvel_x *= -1;
				if (ballvel_y > 0.2) {
					ballvel_y -= 0.2;
				}
				break;
			}
		}
	}

	//plot the ball on the screen
	byte plot_x = (int)(ballpos_x + 0.5f);
	byte plot_y = (int)(ballpos_y + 0.5f);

	_d.drawPixel(plot_x, plot_y, _d.Color333(4, 0, 0));

	//check if a bat missed the ball. if it did, reset the game.
	if ((int)ballpos_x == _pos.x || (int) ballpos_x == _pos.x + _size.x) {
		restart = Time.second(); // to wait for new second before restart
		if (!restart) restart++;
	}
}


byte RGBPongClass::pong_get_ball_endpoint(float tempballpos_x, float  tempballpos_y, float  tempballvel_x, float tempballvel_y) {

	//run prediction until ball hits bat
	while (tempballpos_x > _pos.x + BAT1_X && tempballpos_x < _pos.x + BAT2_X) {
		tempballpos_x += tempballvel_x;
		tempballpos_y += tempballvel_y;
		//check for collisions with top / bottom
		if (tempballpos_y <= _pos.y || tempballpos_y >= _pos.y + _size.y - 1) {
			tempballvel_y *= -1;
		}
	}
	return tempballpos_y;
}

const int16_t RGBPongClass::BAT_HEIGHT =  6;
const int16_t RGBPongClass::BAT1_X    =  2;  // collision detection
const int16_t RGBPongClass::BAT2_X    = 28;
