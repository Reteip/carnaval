
#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_
#include <FastLED.h>

#define VOLTS          5
#define MAX_MA       2000
#define LED_PIN_1    9
#define LED_PIN_2    10
#define LED_PIN_3    11
#define NUM_LEDS    104
#define NUM_LEDS_PER_STRIP 26
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define MENU_LED_1 0
#define MENU_LED_2 1
#define MENU_LED_3 2
#define MENU_LED_4 3
#define MENU_LED_5 4

#define RIPPLE_STEPS 16

const CRGB MenuColors[8] =
		{  CRGB::Cyan, CRGB::Yellow, CRGB::Green, CRGB::Red, 
		CRGB::DarkBlue, CRGB::DarkOrange, CRGB::Purple, CRGB::White };


enum { NONE=0, CENTER = 1, LEFT = 2, RIGHT = 3, FADEUP = 4, FADEDOWN = 5};

void InitLedDriver();

void SetStripOff();

void RainbowProgram(uint8_t index);

void Show_Cycle(uint8_t cycle);

void DoDelay(unsigned long ms);

void Show();
void FillAllLedsSolid( const struct CRGB& color);

void SolidColorProgram(CRGB color, uint8_t program_index,uint16_t programCounter);
void BiertjeProgram(uint16_t programCounter);

#endif