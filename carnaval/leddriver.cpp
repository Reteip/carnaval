#include <Arduino.h>
#include <FastLED.h>
#include "leddriver.h"

#define LED_PIN     5
#define NUM_LEDS    60
#define BRIGHTNESS  32
#define LED_TYPE    WS2812B
#define COLOR_ORDER RGB

#define MENU_LED_1 0
#define MENU_LED_2 1
#define MENU_LED_3 2
#define MENU_LED_4 3
#define MENU_LED_5 4


CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

void InitLedDriver() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
}



void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}
void SetStripOff() {
    fill_solid( currentPalette, 16, CRGB::Black);
    FillLEDsFromPaletteColors( 0);
    FastLED.show();   
}

void RainbowProgram(uint8_t index) {
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
    
  FillLEDsFromPaletteColors( index);

}

void Show() {
	FastLED.show();
}

void DoDelay(unsigned long ms)
{
	FastLED.delay(ms);
}

void Show_Cycle(uint8_t cycle) {
  switch (cycle){
	  case 0:
	  //BiertjeLoop
		leds[MENU_LED_1] = CRGB ( 255, 255, 255 );		
		leds[MENU_LED_2] = CRGB ( 255, 159, 0 );
		leds[MENU_LED_3] = CRGB ( 255, 159, 0 );	

		break;
		//DiscoNormal
		case 1:		
		leds[MENU_LED_4] = CRGB ( 255, 0, 0 );
		leds[MENU_LED_3] = CRGB ( 0, 255, 0 );
		leds[MENU_LED_1] = CRGB ( 0, 0, 255 );
		break;
		case 2:
		//Defibrillator
		leds[MENU_LED_1] = CRGB ( 255, 255, 255 );
		leds[MENU_LED_2] = CRGB ( 255, 255, 255 );
		leds[MENU_LED_3] = CRGB ( 255, 255, 255 );	
		break;
		case 3:
		//Heartbeat
		leds[MENU_LED_1] = CRGB ( 255, 0, 0 );
		leds[MENU_LED_2] = CRGB ( 255, 0, 0 );
		leds[MENU_LED_3] = CRGB ( 255, 0, 0 );
		break;
		case 4:
		//DiscoHooker
		leds[MENU_LED_1] = CRGB ( 200, 0, 0 );
		leds[MENU_LED_3] = CRGB ( 0, 0, 129 );		
		leds[MENU_LED_5] = CRGB ( 200, 129, 0 );
		break;
		case 5:
		//DiscoBlinkOnGameButton
		leds[MENU_LED_1] = CRGB ( 255, 0, 0 );
		leds[MENU_LED_2] = CRGB ( 0, 255, 0 );
		leds[MENU_LED_3] = CRGB ( 0, 0, 255 );
	
		break;
	
		default:
	
		break;
	}
}