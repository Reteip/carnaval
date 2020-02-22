
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
#define BRIGHTNESS  168
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define MENU_LED_1 0
#define MENU_LED_2 1
#define MENU_LED_3 2
#define MENU_LED_4 3
#define MENU_LED_5 4
 

enum { CENTER = 1, LEFT = 2, RIGHT = 3, FADEUP = 4, FADEDOWN = 5};

void InitLedDriver();

void SetStripOff();

void RainbowProgram(uint8_t index);

void Show_Cycle(uint8_t cycle);

void DoDelay(unsigned long ms);

void Show();

void SolidColorProgram(CRGB color);
void BiertjeProgram(uint16_t programCounter);

// void colortwinkles();
// void setRandomPixelToOriginalColor();

// CRGB makeBrighter( const CRGB& color, fract8 howMuchBrighter) ;
// CRGB makeDarker( const CRGB& color, fract8 howMuchDarker) ;
// uint8_t getPixelInformation( uint16_t i);
// void setPixelInformation( uint16_t i, uint8_t dir);

// uint8_t getPixelStep( uint16_t i);
// void setPixelStep( uint16_t i, uint8_t step);
#endif