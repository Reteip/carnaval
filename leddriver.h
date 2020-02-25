
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

// const CRGB MenuColors[8] =
// 		{  CRGB::Cyan, CRGB::Yellow, CRGB::Green, CRGB::Red, 
// 		CRGB::DarkBlue, CRGB::DarkOrange, CRGB::Purple, CRGB::White };

const CHSV MenuColors[8] =
		{  CHSV(128,255,255), CHSV(64,255,255), CHSV(96,255,255), CHSV(0,255,255), 
		CHSV(160,255,255),CHSV(24,255,255), CHSV(228,255,255), CHSV(0,0,255) };


enum { NONE=0, CENTER = 1, LEFT = 2, RIGHT = 3, FADEUP = 4, FADEDOWN = 5};

void InitLedDriver();

void SetStripOff();

void RainbowProgram(uint8_t index);

void Show_Cycle(uint8_t cycle);

void DoDelay(unsigned long ms);

void Show();
void ShiftLedsAround(uint8_t end, uint8_t start);
void FillAllLedsSolid( const struct CRGB& color);
void FillAllLedsSolid( const struct CHSV& color);
void FillAllLedsWithDottedRainbow();
void FadeAllLedsBy(uint8_t fadeBy);
void SolidColorProgram(CHSV color, uint8_t program_index,uint16_t programCounter);
void BiertjeProgram(uint16_t programCounter);
void ScreenSaver();

#endif