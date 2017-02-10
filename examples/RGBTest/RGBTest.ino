
#pragma SPARK_NO_PREPROCESSOR 

#include "Particle.h"
SYSTEM_THREAD(ENABLED)

#define  not_PARTICLE_BUILD_IDE_
#if defined(_PARTICLE_BUILD_IDE_)
#  include "SparkIntervalTimer/SparkIntervalTimer.h"
#  include "RGBmatrixPanel/RGBmatrixPanel.h"
#  include "Adafruit_mfGFX/Adafruit_mfGFX.h"
#else
#  include "SparkIntervalTimer.h"
#  include "RGBmatrixPanel.h"
#  include "Adafruit_mfGFX.h"
#endif

#include "RGBAnimation.h"
#include "RGBAnalogClock.h"
#include "RGBNormal.h"
#include "RGBPlasma.h"
#include "RGBPong.h"
#include "RGBRainbow.h"
#include "RGBWeather.h"

#include "RGBSwypePath.h"

#include "WeatherInfo.h"

/**************** dummy includes for WebIDE to stop arguing *************************/
#include "RGBUtility.h"
#include "RGBAnimationPath.h"
/************************************************************************************/

#define A    A0
#define B    A1
#define C    A2
#define D    RX
#define LAT  TX 
#define CLK  D6
#define OE   D7


RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true, 32);
//RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true, 64);

RGBAnimationClass* rgbAnimation[] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
const int numAnimations = sizeof(rgbAnimation)/sizeof(rgbAnimation[0]);

WeatherInfoClass   myWeather("Halifax,GB", "weather_hook");
RGBSwypePathClass* swypePath;

int setCode(String cmd)
{
    int code = cmd.toInt();
    for (int i=0; i<8; i++)
      myWeather.code[i] = code+i;
    return code;
}

void updateWeather()
{
    myWeather.refreshWeather();
    //((RGBWeatherAnimationClass*)rgbAnimation[1])->refreshWeather();
}

Timer weatherUpdate(15*60*1000, updateWeather);

int weatherAnimationIndex = -1;

//Timer displayUpdate(50, updateAnimations);
void updateAnimations()
{
  static uint32_t msNextDay = 0;
  static int day;

  if (millis()-msNextDay > 5000)
  {
    int state = swypePath->getState();
    switch(state)
    {
      case RGBSwypePathClass::STOPPED:
        ((RGBSwypePathClass*)swypePath)->start();
        break;
      case RGBSwypePathClass::SWYPEIN:
        msNextDay = millis();
        day++;
        day %= 7;
        break;
      default:
        break;
    }
  }

  matrix.fillScreen(0); // ensure blank canvas
  for (int i=0; i<numAnimations; i++)
  {
    if (rgbAnimation[i]) rgbAnimation[i]->drawFrame(day);
  }
  
  matrix.swapBuffers(true);
}

void setup() 
{
  Time.zone(+0);

  Particle.function("testCode", setCode);
  randomSeed(analogRead(DAC));

  waitFor(Particle.connected, 30000);

  matrix.begin();
  matrix.setTextSize(1);

  int i = 0;
  // how to instantiate the different animations
  //rgbAnimation[i++] = new RGBPongClass(matrix, 0, 0);
  //rgbAnimation[i++] = new RGBPongClass(matrix, 0, 16);
  //rgbAnimation[i++] = new RGBPlasmaClass(matrix);
  //rgbAnimation[i++] = new RGBRainbowClass(matrix, 0, 0, 32, 32, 32, 32, RGBRainbowClass::HORIZONTAL);
  //rgbAnimation[i++] = new RGBWeatherClass(matrix,  0, 16, "Ottawa,CA", "weather_hook");
  //rgbAnimation[i++] = new RGBRainbowClass(matrix, 0, 0, 8, 8, 150, 20, true, RGBRainbowClass::HORIZONTAL);
  //rgbAnimation[i++] = new RGBRainbowClass(matrix, 24, 0, 8, 8, 150, 20, true,  RGBRainbowClass::VERTICAL);
  //rgbAnimation[i++] = new RGBRainbowClass(matrix, 0, 24, 8, 8, 150, 20, true,  RGBRainbowClass::TOPRIGHT_BOTTOMLEFT);
  //rgbAnimation[i++] = new RGBRainbowClass(matrix, 24, 24, 8, 8, 150, 20, true,  RGBRainbowClass::TOPLEFT_BOTTOMRIGHT);
  //rgbAnimation[i++] = new RGBRainbowClass(matrix, 49, 20, RGBRainbowClass::CENTER_RAY);
  //rgbAnimation[i++] = new RGBRainbowClass(matrix, 7, 7, 18, 18, 100, 20, true, RGBRainbowClass::CENTER_BOX);
  rgbAnimation[i++] = new RGBNormalClass(matrix, 0, 0, RGBNormalClass::SMALL);
  rgbAnimation[weatherAnimationIndex = i++] = new RGBWeatherClass(matrix, 8, 16, &myWeather);

  //swypePath = new RGBSwypePathClass(rgbAnimation[weatherAnimationIndex], RGBSwypePathClass::DOWN);
  //rgbAnimation[weatherAnimationIndex]->add(swypePath);
  swypePath = (RGBSwypePathClass*)rgbAnimation[weatherAnimationIndex]->add(new RGBSwypePathClass(RGBSwypePathClass::RIGHT));
/*
  rgbAnimation[i++] = new RGBAnalogClockClass(matrix, 0,  0, 32, 32, true
                                           ,matrix.Color444( 15, 15, 15)
                                           ,matrix.Color444(  0,  0, 15)
                                           ,matrix.Color444(  0, 15,  0)
                                           ,matrix.Color444(  7,  0,  0)  
                                           ); 
*/

  //rgbAnimation[0]->add(RGBSwypePathClass());

  updateWeather();
  weatherUpdate.start();
  //displayUpdate.start();
}

void loop() 
{
    static uint32_t msPublish;
    static uint32_t msAnimation = 0;

    if (millis() - msAnimation < 30) return;
    msAnimation = millis();
    updateAnimations();
}
