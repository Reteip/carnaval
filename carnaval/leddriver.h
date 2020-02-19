
#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_
#include <FastLED.h>

void InitLedDriver();

void SetStripOff();

void RainbowProgram(uint8_t index);

void Show_Cycle(uint8_t cycle);

void ExecuteProgram(uint8_t cycle, uint16_t programCounter);

void DoDelay(unsigned long ms);

void Show();

void colortwinkles();
void setRandomPixelToOriginalColor();

CRGB makeBrighter( const CRGB& color, fract8 howMuchBrighter) ;
CRGB makeDarker( const CRGB& color, fract8 howMuchDarker) ;
uint8_t getPixelInformation( uint16_t i);
void setPixelInformation( uint16_t i, uint8_t dir);

uint8_t getPixelStep( uint16_t i);
void setPixelStep( uint16_t i, uint8_t step);
#endif