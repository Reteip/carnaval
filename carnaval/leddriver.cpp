#include <Arduino.h>
#include <FastLED.h>
#include "leddriver.h"
#define VOLTS          5
#define MAX_MA       450
#define LED_PIN     5
#define NUM_LEDS    105
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define MENU_LED_1 0
#define MENU_LED_2 1
#define MENU_LED_3 2
#define MENU_LED_4 3
#define MENU_LED_5 4


CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

void InitLedDriver() {
	FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS,MAX_MA);
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

void Biertje(uint16_t loopindex)
{   
	uint8_t index = loopindex / 6;
	if (index > NUM_LEDS) index = NUM_LEDS;
	uint8_t foam = (index / 5) + 1;
    for( int i = 0; i < NUM_LEDS; i++) {
		if (i < index) 
		{
			if (i > (index - foam))
			{
				leds[i] = CRGB::White;
			} else
			{
	        	leds[i] = CRGB ( 255, 159, 0 );
			}
			
		} else
		{
			leds[i] = CRGB::Black;
		}
		
    }
	
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
		//DiscoBlinkOnProgramButton
		leds[MENU_LED_1] = CRGB ( 255, 0, 0 );
		leds[MENU_LED_2] = CRGB ( 0, 255, 0 );
		leds[MENU_LED_3] = CRGB ( 0, 0, 255 );
		break;
		case 6:
		leds[MENU_LED_1] = CRGB::Cyan;
		leds[MENU_LED_2] = CRGB::Cyan;
		leds[MENU_LED_3] = CRGB::Cyan;
		break;
		case 7:
		leds[MENU_LED_1] = CRGB::Yellow;
		leds[MENU_LED_2] = CRGB::Yellow;
		leds[MENU_LED_3] = CRGB::Yellow;
		break;
		case 8:
		leds[MENU_LED_1] = CRGB::Green;
		leds[MENU_LED_2] = CRGB::Green;
		leds[MENU_LED_3] = CRGB::Green;
		break;
		case 9:
		leds[MENU_LED_1] = CRGB::Red;
		leds[MENU_LED_2] = CRGB::Red;
		leds[MENU_LED_3] = CRGB::Red;
		break;
		case 10:
		leds[MENU_LED_1] = CRGB::DarkBlue;
		leds[MENU_LED_2] = CRGB::DarkBlue;
		leds[MENU_LED_3] = CRGB::DarkBlue;
		break;
		case 11:
		leds[MENU_LED_1] = CRGB::DarkOrange;
		leds[MENU_LED_2] = CRGB::DarkOrange;
		leds[MENU_LED_3] = CRGB::DarkOrange;
		break;
		case 12:
		leds[MENU_LED_1] = CRGB::Purple;
		leds[MENU_LED_2] = CRGB::Purple;
		leds[MENU_LED_3] = CRGB::Purple;
		break;
		case 13:
		leds[MENU_LED_1] = CRGB::White;
		leds[MENU_LED_2] = CRGB::White;
		leds[MENU_LED_3] = CRGB::White;
		break;
		default:
		break;
		// Wit, roze, oranje, blauw, rood,groen, geel, lichtblauw
	}

	
}

void ExecuteProgram(uint8_t cycle, uint16_t loopindex)
{
	 switch (cycle){
	  case 0:
		Biertje(loopindex);
		break;
		//DiscoNormal
		case 1:		
		Serial.println("Case 1, solid white");

		fill_solid( currentPalette, 16, CRGB::Yellow);
		currentBlending = NOBLEND;
		FillLEDsFromPaletteColors( 0);
		break;
		case 2:
		currentPalette = LavaColors_p;
		currentBlending = LINEARBLEND;
    
  		FillLEDsFromPaletteColors( loopindex);


		//Defibrillator
		break;
		case 3:
		//Heartbeat
		currentPalette = RainbowColors_p;
		currentBlending = LINEARBLEND;
    
  		FillLEDsFromPaletteColors( loopindex);

		break;
		case 4:
		//DiscoHooker
		leds[MENU_LED_1] = CRGB ( 200, 0, 0 );
		leds[MENU_LED_3] = CRGB ( 0, 0, 129 );		
		leds[MENU_LED_5] = CRGB ( 200, 129, 0 );
		break;
		case 5:
		//DiscoBlinkOnProgramButton
		leds[MENU_LED_1] = CRGB ( 255, 0, 0 );
		leds[MENU_LED_2] = CRGB ( 0, 255, 0 );
		leds[MENU_LED_3] = CRGB ( 0, 0, 255 );
		break;
		case 6:
		leds[MENU_LED_1] = CRGB::Cyan;
		leds[MENU_LED_2] = CRGB::Cyan;
		leds[MENU_LED_3] = CRGB::Cyan;
		break;
		default:
		break;
	 }
}