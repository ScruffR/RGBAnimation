#include "RGBWeatherAnimation.h"

void RGBWeatherAnimationClass::draw()
{
  static int32_t  subFrame = 0;
  static uint32_t msIncSubFrame = 0;

  const uint8_t*  icon[3] = { NULL, NULL, NULL }; // up to three stacked icons
  int16_t         foreColor[3] = { _d.Color333(7,7,7), _d.Color333(7,7,7), _d.Color333(7,7,7) };
  int16_t         backColor = _d.Color333(0, 0, 0); // black
  bool            precipitate = false;
  int16_t         precipColor = _d.Color333(0, 0, 7); // blue
  char            text[8] = "";
  int16_t         textColor = _d.Color333(7, 7, 7); // white
  int             intensity = _w->code[_curFrame] % 10;

  if (millis() - msIncSubFrame > 20)
  {
    msIncSubFrame = millis();
    subFrame++;
  }

  switch (_w->code[_curFrame] / 100)
  {
  case 0: // 0xx no valid code
    break;
  case 2: // 2xx Thunderstorm
    icon[0] = cloud_outline;
    icon[1] = lightning;
    precipitate = true;

    if (subFrame % random(5, 8))
      foreColor[1] = random(16, 32) << 11 | random(8, 32) << 5 | random(0, 3);
    else
      foreColor[1] = backColor;
    break;
  case 3: // 3xx Drizzle
    icon[0] = cloud;
    foreColor[0] = _d.Color444(13, 13, 13); // light gray
    icon[1] = cloud_outline;
    foreColor[1] = _d.Color444(7, 7, 7); // gray
    precipColor = _d.Color444(5, 5, 15); // light blue
    precipitate = true;
    break;
  case 5: // 5xx Rain
    icon[0] = cloud;
    foreColor[0] = _d.Color444(5, 5, 5); // dark gray
    icon[1] = cloud_outline;
    foreColor[1] = _d.Color444(7, 7, 7); // gray
    precipitate = true;
    break;
  case 6: // 6xx Snow
    icon[0] = cloud;
    icon[1] = cloud_outline;
    foreColor[0] = (intensity < 3 ? _d.Color444(7, 7, 7) : _d.Color444(12, 12, 12));
    //foreColor[1] = _d.Color444(15,15,15);
    precipColor = _d.Color444(13, 13, 13);     // light gray
    precipitate = true;
    break;
  case 7: // Atmosphere (e.g. fog,mist,smoke, ...)
    icon[0] = fog;
    foreColor[0] = _d.Color444(5, 5, 5); // dark gray
    backColor = _d.Color444(3, 3, 3);    // dark dark gray
    break;
  case 8: // 800 Clear 80x Clouds
    switch (intensity)
    {
    case 0:
      icon[0] = big_sun;
      foreColor[0] = _d.Color444(12, 12, 0);
      backColor = _d.Color444(3, 3, 12);
      break;
    case 1:
      icon[0] = big_sun;
      foreColor[0] = _d.Color444(7, 7, 0);
      backColor = _d.Color444(6, 6, 12);
      break;
    case 2:
    case 3:
      icon[0] = small_sun;
      foreColor[0] = _d.Color444(12, 6, 0);
      icon[1] = cloud;
      //foreColor[1] = _d.Color444(15,15,15);
      icon[2] = cloud_outline;
      foreColor[2] = _d.Color444(7, 7, 7);
      backColor = _d.Color444(3, 3, 7);
      break;
    case 4:
    default:
      icon[0] = small_sun;
      foreColor[0] = _d.Color444(10, 5, 0);
      icon[1] = cloud;
      //foreColor[1] = _d.Color444(15,15,15);
      icon[2] = cloud_outline;
      foreColor[2] = _d.Color444(5, 5, 5);
      backColor = _d.Color444(5, 5, 7);
      break;
    }
    break;
  case 9: // 90x Extreme, 9xx Additional
    icon[0] = extreme;
    foreColor[0] = _d.Color444(15, 0, 0);              // red
    backColor = _d.Color444(7, 7, 0);                  // yellow
    if (intensity == 6)  // hail
    {
      precipitate = true;
      precipColor = _d.Color444(7, 7, 13);             // icy blue
    }
    break;
  default:
    snprintf(text, sizeof(text), "%03d", _w->code[_curFrame]);
    break;
  }

  // clear background with set color
  _d.fillRect(_pos.x, _pos.y, _size.x, _size.y, backColor);

  // draw up to three icons
  for (int i = 0; i < 3 && icon[i]; i++)
    _d.drawBitmap(_pos.x, _pos.y, icon[i], _size.x, _size.y, foreColor[i]);

  // draw and scroll precipitation bitmap
  if (precipitate)
  {
    if (precipitation[0] != _curFrame)
    { // if we have a new day, rebuild the rain pattern
      precipitation[0] = _curFrame;
      for (int i = 1; i <= WEATHER_PRECIP_SCANLINES; i++)
      {
        precipitation[i] = 0;
        for (int b = 0; b <= intensity; b++)
          precipitation[i] |= 1 << random(0, 15); // with small endian would be (1,13), but
        precipitation[i] &= 0b1111111000111111;  // due to big endian the bitmap edges are in the middle
        precipitation[i + WEATHER_PRECIP_SCANLINES] = precipitation[i];
      }
    }
    _d.drawBitmap(_pos.x, _pos.y + (_size.y - WEATHER_PRECIP_SCANLINES), (uint8_t*)&precipitation[WEATHER_PRECIP_SCANLINES - (subFrame % WEATHER_PRECIP_SCANLINES)], _size.x, WEATHER_PRECIP_SCANLINES, precipColor);
  }

  // draw text (if any)
  if (text[0])
    print3x6(_pos.x + 2, _pos.y + 10, text, textColor);


  // map temp to color
  int dmy = map(constrain(_w->temp[_curFrame], -10, 30), -15, 35, 0, 31);
  textColor = (dmy < 8 ? 0 : 31) << 11 | (dmy < 8 ? dmy : 31 - dmy) << 6 | (dmy < 8 ? 31 : 0) << 0;

  // print day temperature
  snprintf(text, sizeof(text), "%.0f%\x01", round(_w->temp[_curFrame])); // \x01 for ° symbol (otherwise 176 or 0xB0 which is non-present in font)
  print3x6(_pos.x + 16 - strlen(text) * 3, _pos.y + 1, text, _d.Color444(0, 0, 0));
  print3x6(_pos.x + 16 - strlen(text) * 3 - 1, _pos.y, text, textColor);

  if (_curFrame)
    _d.drawLine(_pos.x, _pos.y + _size.y - 1, _pos.x + _curFrame - 1, _pos.y + _size.y - 1, _d.Color444(10, 3, 0));   // indicate which day of the forcast with a single pixle
}

const uint8_t RGBWeatherAnimationClass::cloud[] PROGMEM = {
  0b00000000,0b00000000 ,
  0b00000000,0b11100000 ,
  0b00000001,0b11111000 ,
  0b00111011,0b11111100 ,
  0b01111111,0b11111110 ,
  0b11111111,0b11111111 ,
  0b11111111,0b11111111 ,
  0b01111111,0b11111111 ,
  0b00111111,0b11111110 ,
  0b00000000,0b00000000 ,
  0b00000000,0b00000000 ,
  0b00000000,0b00000000 ,
  0b00000000,0b00000000 ,
  0b00000000,0b00000000 ,
  0b00000000,0b00000000 ,
  0b00000000,0b00000000
};

const uint8_t RGBWeatherAnimationClass::cloud_outline[] PROGMEM = {
  0b00000000,0b00000000 ,
  0b00000000,0b11100000 ,
  0b00000001,0b00011000 ,
  0b00111010,0b00000100 ,
  0b01000100,0b00000010 ,
  0b10000000,0b00000001 ,
  0b10000000,0b00000001 ,
  0b01000000,0b00000001 ,
  0b00111111,0b11111110 ,
  0b00000000,0b00000000 ,
  0b00000000,0b00000000 ,
  0b00000000,0b00000000 ,
  0b00000000,0b00000000 ,
  0b00000000,0b00000000 ,
  0b00000000,0b00000000 ,
  0b00000000,0b00000000 ,
};

const uint8_t RGBWeatherAnimationClass::small_sun[] PROGMEM = {
  0b00000000, 0b00000000,
  0b00001110, 0b00000000,
  0b00111111, 0b10000000,
  0b00111111, 0b10000000,
  0b01111111, 0b11000000,
  0b01111111, 0b11000000,
  0b01111111, 0b11000000,
  0b00111111, 0b10000000,
  0b00111111, 0b10000000,
  0b00001110, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

const uint8_t RGBWeatherAnimationClass::big_sun[] PROGMEM = {
  0b10000001, 0b00000010,
  0b01000001, 0b00000100,
  0b00100000, 0b00001000,
  0b00000011, 0b10000000,
  0b00001111, 0b11100000,
  0b00001111, 0b11100000,
  0b00011111, 0b11110000,
  0b11011111, 0b11110110,
  0b00011111, 0b11110000,
  0b00001111, 0b11100000,
  0b00001111, 0b11100000,
  0b00000011, 0b10000000,
  0b00100000, 0b00001000,
  0b01000001, 0b00000100,
  0b10000001, 0b00000010,
  0b00000001, 0b00000000,
};

const uint8_t RGBWeatherAnimationClass::lightning[] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b11100000,
  0b00000001, 0b11000000,
  0b00000000, 0b11100000,
  0b00000000, 0b01110000,
  0b00000000, 0b11100000,
  0b00000001, 0b11000000,
  0b00000011, 0b00000000,
  0b00000100, 0b00000000,
};

const uint8_t RGBWeatherAnimationClass::fog[] PROGMEM = {
  0b00000000, 0b00000000,
  0b01100110, 0b01100110,
  0b00011001, 0b10011000,
  0b01000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00110011, 0b00110011,
  0b11001100, 0b11001100,
  0b00000000, 0b00000000,
  0b00000000, 0b00100000,
  0b00011001, 0b10011000,
  0b01100110, 0b01100110,
  0b00000000, 0b00000000,
  0b00000000, 0b10000000,
  0b00110011, 0b00110011,
  0b11001100, 0b11001100,
  0b00000000, 0b00000000,
};

const uint8_t RGBWeatherAnimationClass::extreme[] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b10000000,
  0b00000001, 0b01000000,
  0b00000001, 0b01000000,
  0b00000010, 0b10100000,
  0b00000010, 0b10100000,
  0b00000100, 0b10010000,
  0b00000100, 0b10010000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000,
  0b00010000, 0b10000100,
  0b00010000, 0b00000100,
  0b00111111, 0b11111110,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

