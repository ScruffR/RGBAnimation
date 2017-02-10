#pragma once

#include "RGBAnimation.h"
#include "WeatherInfo.h"
#include "Particle.h"

#define WEATHER_PRECIP_SCANLINES (7)

//static const char daynames[7][4] = { "Su", "Mo","Tu", "We", "Th", "Fr", "Sa" };

class RGBWeatherClass: public RGBAnimationClass
{
public:
  RGBWeatherClass(RGBmatrixPanel & display, int16_t x, int16_t y, WeatherInfoClass* weather) 
  : RGBAnimationClass(display, x, y, 16, 16, 7, 0), _w(weather), _wCleanup(false) { };
  RGBWeatherClass(RGBmatrixPanel & display, int16_t x, int16_t y, const char* city, const char* webhook) 
  : RGBAnimationClass(display, x, y, 16, 16, 7, 0), _w(new WeatherInfoClass(city, webhook)), _wCleanup(true) { };
  virtual ~RGBWeatherClass() { if(_wCleanup) delete(_w); };
  
  virtual CAPABILITIES getCaps() { return CAN_MOVE; }

  bool refreshWeather() { return (_w ? _w->refreshWeather() : false); };

private:
  virtual void draw();
  
  WeatherInfoClass*    _w;
  bool                 _wCleanup;        // will _w need cleaning up in destructor?
  
  // weather bitmaps
               uint16_t precipitation[2*WEATHER_PRECIP_SCANLINES+1] = { 0 };
  static const uint8_t  cloud[];
  static const uint8_t  cloud_outline[]; 
  static const uint8_t  big_sun[];
  static const uint8_t  small_sun[];
  static const uint8_t  lightning[];
  static const uint8_t  fog[];
  static const uint8_t  extreme[];
};
