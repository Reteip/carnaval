#include <Arduino.h>
#include "leddriver.h"

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


CRGB leds[NUM_LEDS];
CRGB ledsOriginal[NUM_LEDS];
uint8_t directionFlags[NUM_LEDS];
uint8_t stepFlags[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

enum { CENTER = 1, LEFT = 2, RIGHT = 3, FADEUP = 4, FADEDOWN = 5};

void InitLedDriver() {
	FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS,MAX_MA);
    FastLED.addLeds<LED_TYPE, LED_PIN_1, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP).setCorrection( TypicalLEDStrip );
	FastLED.addLeds<LED_TYPE, LED_PIN_2, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection( TypicalLEDStrip );
	FastLED.addLeds<LED_TYPE, LED_PIN_3, COLOR_ORDER>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP * 2).setCorrection( TypicalLEDStrip );

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


void ProcessRipple()
{
	uint8_t BrightAmount = 16;
	for( int pos = 0; pos < NUM_LEDS; pos++) { 
		if (directionFlags[pos] == CENTER)
		{
			directionFlags[pos - 1] = LEFT;
			stepFlags[pos - 1] = 2;
			directionFlags[pos + 1] = RIGHT;
			stepFlags[pos +1 ] = 2;
			directionFlags[pos] = FADEUP;
		}
		if (directionFlags[pos] == LEFT) {
			if (stepFlags[pos] > 1) {
				directionFlags[pos - 1] = LEFT;
				stepFlags[pos - 1] = stepFlags[pos] - 1; 
			}
			directionFlags[pos] = FADEUP;
			stepFlags[pos] = 16;
			ledsOriginal[pos] = leds[pos];

		}
		if (directionFlags[pos] == RIGHT) {
			if (stepFlags[pos] > 1) {
				directionFlags[pos + 1] = RIGHT;
				stepFlags[pos + 1] = stepFlags[pos] - 1; 
			}
			directionFlags[pos] = FADEUP;
			stepFlags[pos] = 16;
			ledsOriginal[pos] = leds[pos];
		}
		if (directionFlags[pos] == FADEUP) 
		{
			if (stepFlags[pos] > 0)
			{
				if ((leds[pos].r + BrightAmount < 255) || (leds[pos].g + BrightAmount < 255) || (leds[pos].b + BrightAmount < 255))
				{
					leds[pos].r = min(leds[pos].r + BrightAmount, 255);
					leds[pos].g = min(leds[pos].g + BrightAmount, 255);
					leds[pos].b = min(leds[pos].b + BrightAmount, 255);
					stepFlags[pos]-=1;
				}
				else {
					directionFlags[pos] = FADEDOWN;
					stepFlags[pos] = 16 - stepFlags[pos];
				}
			}
		}
		if (directionFlags[pos] == FADEDOWN)
		{
			if (stepFlags[pos] > 0)
			{
				leds[pos].r = max(leds[pos].r - BrightAmount, ledsOriginal[pos].r);
				leds[pos].g = max(leds[pos].g - BrightAmount, ledsOriginal[pos].g);
				leds[pos].b = max(leds[pos].b - BrightAmount, ledsOriginal[pos].b);
				stepFlags[pos]-=1;
			} else
			{
				directionFlags[pos] = 0;
			}
		}
	}
}

void AddRipple(uint8_t center)
{
	if (directionFlags[center] == 0)
	{
		directionFlags[center] = 1;
		ledsOriginal[center] = leds[center];
		stepFlags[center] = 16;
	}
}

void ReverseDoubleLeds() 
{
	for( int i = 0; i < NUM_LEDS / 2; i++) { 
		leds[NUM_LEDS - 1 - i] = leds[i];
		ledsOriginal[NUM_LEDS - 1 - i] = ledsOriginal[i];
	}
}

void CopyStrip(uint8_t origin, uint8_t target) {
	for( int pos = origin; pos < NUM_LEDS_PER_STRIP; pos++) { 
		leds[pos + target] = leds[pos];
	}
}

void ShiftAllLedsUp()
{
	for (int pos = NUM_LEDS; pos > 0; pos--)
	{
		leds[pos] = leds[pos - 1];
	}
}

void Biertje(uint16_t programCounter)
{   
	if (programCounter % 10 == 0)
	{
		uint16_t index = programCounter / 10;
		if (index < NUM_LEDS_PER_STRIP)
		{
			ShiftAllLedsUp();
			
			leds[0] = CRGB ( 255, 159, 0 );
			uint8_t foam = (index / 100.0) * 30;

			leds[NUM_LEDS_PER_STRIP] = CRGB ( 255, 159, 0 );
			//leds[2 * NUM_LEDS_PER_STRIP] = CRGB ( 255, 159, 0 );
			for (int i = 0; i < foam; i++) 
			{
				leds[index - i] = CRGB::White;
				leds[index + NUM_LEDS_PER_STRIP - i] = CRGB::White;
			}
		} 
		if (index >= NUM_LEDS_PER_STRIP && index < 2 * NUM_LEDS_PER_STRIP)
		{
			
		}
	}
}	
// 	uint16_t index = programCounter / 10;
// 	if (index > NUM_LEDS) {
// 		if (random8(5) == 0) {
// 			setRandomPixelToOriginalColor();
//      		fadeLightBy(leds, NUM_LEDS, 1);

// 		}
//  	} else 
// 	{
		
// 		uint8_t beerCounter = 0;
// 		if (index > NUM_LEDS_PER_STRIP) uint8_t beerCounter = 1;
// 		if (index > 2 * NUM_LEDS_PER_STRIP) uint8_t beerCounter = 2;

// 		uint8_t foam = ((index - beerCounter * NUM_LEDS_PER_STRIP) / 3) + 1;
// 		for( int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
// 			if (i < index) 
// 			{
// 				if (i > (index - foam))
// 				{
// 					leds[i + beerCounter * NUM_LEDS_PER_STRIP] = CRGB::White;
					
// 				} else
// 				{
// 					leds[i + beerCounter * NUM_LEDS_PER_STRIP] = CRGB ( 255, 159, 0 );
// 				}
				
// 			} 
// 			ledsOriginal[i + NUM_LEDS_PER_STRIP] = ledsOriginal[i] = leds[i];
// 		}
		
// 		//ReverseDoubleLeds();
// 	}

	
// }

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
		// Wit, roze, oranje, blauw, rood,groen, geel, lichtblauw
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

		
	}
	ReverseDoubleLeds();
	// leds[NUM_LEDS-1-MENU_LED_1] = leds[MENU_LED_1];
	// leds[NUM_LEDS-1-MENU_LED_2] = leds[MENU_LED_2];
	// leds[NUM_LEDS-1-MENU_LED_3] = leds[MENU_LED_3];
	// leds[NUM_LEDS-1-MENU_LED_4] = leds[MENU_LED_4];
	// leds[NUM_LEDS-1-MENU_LED_5] = leds[MENU_LED_5];
}

void ExecuteProgram(uint8_t cycle, uint16_t programCounter)
{
	 switch (cycle){
	  case 0:
		Biertje(programCounter);
		break;
		//DiscoNormal
		case 1:		
		Serial.println("Case 1, solid white");

		fill_solid( currentPalette, 16, CRGB::Yellow);
		currentBlending = NOBLEND;
		FillLEDsFromPaletteColors( 0);
		break;
		case 2:
		//Defibrillator
		break;
		case 3:
		//Heartbeat
		currentPalette = RainbowColors_p;
		currentBlending = LINEARBLEND;
    
  		FillLEDsFromPaletteColors( programCounter);

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
			if (programCounter == 1)
			{
				fill_solid(leds, NUM_LEDS, CRGB::Cyan);
			}
			EVERY_N_MILLISECONDS( random16(500, 2000) ) { 
				AddRipple(random8(NUM_LEDS));
			}
			ProcessRipple();	
		break;
		case 7:
		fill_solid(leds, NUM_LEDS, CRGB::Yellow);
		break;
		case 8:
		fill_solid(leds, NUM_LEDS, CRGB::Green);
		break;
		case 9:
		fill_solid(leds, NUM_LEDS, CRGB::Red);
		break;
		case 10:
		fill_solid(leds, NUM_LEDS, CRGB::DarkBlue);
		break;
		case 11:
		fill_solid(leds, NUM_LEDS, CRGB::DarkOrange);
		break;
		case 12:
		fill_solid(leds, NUM_LEDS, CRGB::Purple);
		break;
		case 13:
		fill_solid(leds, NUM_LEDS, CRGB::White);
		break;


		default:
		break;
	 }
}


void setRandomPixelToOriginalColor()
{
	int pos = random16(NUM_LEDS);
	leds[pos] = ledsOriginal[pos];
}
