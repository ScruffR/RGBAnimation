#include "WeatherInfo.h"

WeatherInfoClass::WeatherInfoClass(const char* city, const char* webhook) 
{
  _webhook     = strdup(webhook);
  _city        = strdup(city);
  _errorCount  = -1;
  _lastPublish = -WEATHER_PUBLISH_DELAY;
  memset(temp, 0, sizeof(temp));
  memset(code, 0, sizeof(code));
}

WeatherInfoClass::~WeatherInfoClass()
{
  //Particle.unsubscribe();
  if (_webhook) free((void*)_webhook);
  if (_city   ) free((void*)_city   );
}

bool WeatherInfoClass::begin()
{
  char szResponseEvent[strlen(_webhook) + 16]; // to hold "hook-response/" + _webhook

  if (_webhook && _errorCount >= 0) return true;  // already set up
  
  snprintf(szResponseEvent, sizeof(szResponseEvent), "hook-response/%s", _webhook);

  if (Particle.subscribe(szResponseEvent, &WeatherInfoClass::processWeather, this, MY_DEVICES))
  { 
    _errorCount = 0;
    refreshWeather();
    return true;
  }
  else
  {
    if (_webhook) free((void*)_webhook);
    if (_city   ) free((void*)_city   );
    return false;
  }
}

bool WeatherInfoClass::refreshWeather()
{
  if (!begin()) return false;  // make sure we've been setup properly already 

  if(_webhook && millis() - _lastPublish >= WEATHER_PUBLISH_DELAY) // at least 10 sec between two refreshes
  {
    _lastPublish = millis();
    if (Particle.connected() && Particle.publish(_webhook, _city, PRIVATE))
    {
      return true;
    }
    else if(Particle.connected())
    {
      if (++_errorCount > WEATHER_MAX_EVENT_ERRORS)
      { 
        System.reset();
      }
    }
  }
  return false;
}

void WeatherInfoClass::processWeather(const char* event, const char* data)
{
  char _data[strlen(data)+1];
  if (_city && !strstr(data, _city)) return;  // don't bother if this is not the requested city 

  strncpy(_data, data, sizeof(_data));
  memset(temp, 0, sizeof(temp));
  memset(code, 0, sizeof(code));

  // parsing string like this: city,country<18.85~800/17.34~802/15.96~800/16.02~800/17.33~501/16.83~500/11.17~501/>
  strtok(_data, "<");
  for (int item = 0; item < 7; item++)
  {
    temp[item] = atof(strtok(NULL, "~"));
    code[item] = atoi(strtok(NULL, "/"));
    if (code[item] == 0)
    { // since id 0 is not allowed, trigger another refresh
      if (++_errorCount > WEATHER_MAX_EVENT_ERRORS)
      {
          System.reset();
      }
      memset(temp, 0, sizeof(temp));
      memset(code, 0, sizeof(code)); 
      return;
    }
  }

  _errorCount = 0;
}
