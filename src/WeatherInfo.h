#pragma once

#include "Particle.h"
/*
{
"event": "<--- ADD YOUR OWN EVENT NAME HERE --->",
"url": "http://api.openweathermap.org/data/2.5/forecast/daily",
"requestType": "POST",
"headers": null,
"query": {
    "q": "{{PARTICLE_EVENT_VALUE}}",
    "mode": "json",
    "units": "metric",
    "cnt": 7,
    "appid": "<--- ADD YOUR OpenWeatherMap KEY HERE --->"
    },
"responseTemplate": "{{city.name}},{{city.country}}/<{{#list}}{{temp.day}}~{{#weather}}{{id}}{{/weather}}/{{/list}}>",
"json": null,
"auth": null,
"noDefaults": true,
"devideid": null,
"mydevices": true
}
*/

#define WEATHER_PUBLISH_DELAY         (10000)
#define WEATHER_MAX_EVENT_ERRORS          (3)

class WeatherInfoClass 
{
public:
  WeatherInfoClass(const char* city, const char* webhook);
  virtual ~WeatherInfoClass();
  
  bool         begin();
  bool         refreshWeather();
  virtual void processWeather(const char* event, const char* data);

  int          code[8];          // weather codes
  float        temp[8];          // temperatures

private:
  const char*  _webhook;         // event name for webhook
  const char*  _city;            // location for weather (must be equal to "{{city.name}},{{city.country}}" of webhook response)
  uint32_t     _lastPublish;     // for rate limiting of webhook triggers
  int          _errorCount;      // counts how often a publish didn't work or we got a corrupt webhook response
};
