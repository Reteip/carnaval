#include <Arduino.h>
#include "leddriver.h"

CRGB leds[NUM_LEDS];
CRGB ledsOriginal[NUM_LEDS];
uint8_t directionFlags[NUM_LEDS];
uint8_t stepFlags[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

void InitLedDriver() {
	FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS,MAX_MA);
    FastLED.addLeds<LED_TYPE, LED_PIN_1, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP).setCorrection( TypicalLEDStrip );
	FastLED.addLeds<LED_TYPE, LED_PIN_2, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection( TypicalLEDStrip );
	FastLED.addLeds<LED_TYPE, LED_PIN_3, COLOR_ORDER>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP * 2).setCorrection( TypicalLEDStrip );

    FastLED.setBrightness(  BRIGHTNESS );

}

void Show() {
	FastLED.show();
}

void DoDelay(unsigned long ms)
{
	FastLED.delay(ms);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
		if (i%2==0)
		{
        	leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
		}
        colorIndex += 3;
    }
}

void SetStripOff() {
    FillAllLedsSolid(CRGB::Black);
    FastLED.show();   
}

void FillAllLedsSolid( const struct CRGB& color)
{
	for (int pos=0; pos < NUM_LEDS; pos++)
	{
		leds[pos] = ledsOriginal[pos] = color;
	}
}


void FillAllLedsSolid( const struct CHSV& color)
{
	for (int pos=0; pos < NUM_LEDS; pos++)
	{
		leds[pos] = ledsOriginal[pos] = color;
	}
}

void FadeAllLedsBy(uint8_t fadeBy)
{
    fadeLightBy(leds, NUM_LEDS, 10);
}


void RainbowProgram(uint8_t index) {
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
    
  FillLEDsFromPaletteColors( index);

}

void ProcessSparkle(CHSV color)
{
	uint8_t Steps = 16;
	for( int pos = 0; pos < NUM_LEDS; pos++) { 
		if (directionFlags[pos] == CENTER)
		{
			directionFlags[pos - 1] = LEFT;
			stepFlags[pos - 1] = random8(Steps);;
			directionFlags[pos + 1] = RIGHT;
			stepFlags[pos +1 ] = random8(Steps);;
			directionFlags[pos] = FADEUP;
		}
		if (directionFlags[pos] == LEFT) {
			if (stepFlags[pos] > 1) {
				directionFlags[pos - 1] = LEFT;
				stepFlags[pos - 1] = stepFlags[pos] - 1; 
			}
			directionFlags[pos] = FADEUP;
			stepFlags[pos] = random8(Steps);
		}
		if (directionFlags[pos] == RIGHT) {
			if (stepFlags[pos] > 1) {
				directionFlags[pos + 1] = RIGHT;
				stepFlags[pos + 1] = stepFlags[pos] - 1; 
			}
			directionFlags[pos] = FADEUP;
			stepFlags[pos] = random8(Steps);
		}
		if (directionFlags[pos] == FADEUP) 
		{
			if (stepFlags[pos] == Steps)
			{
				leds[pos] = color;
			}

			if (stepFlags[pos] > 0)
			{
				stepFlags[pos]-=1;
			} else 
			{
				directionFlags[pos] = FADEDOWN;
				leds[pos] = ledsOriginal[pos];
				stepFlags[pos] = Steps - stepFlags[pos];
			}
		}
		if (directionFlags[pos] == FADEDOWN)
		{
			if (stepFlags[pos] > 0)
			{
				leds[pos] = color;
				stepFlags[pos]-=1;
			} else
			{
				leds[pos] = ledsOriginal[pos];
				directionFlags[pos] = 0;
			}
		}
	}
}

void ProcessColorSparkle(CHSV color)
{
	for( int pos = 0; pos < NUM_LEDS; pos++) { 
		
			CHSV newColor = color;
			newColor.s = random8(190,255);
			leds[pos] = newColor;
		
	}
}

void ProcessRipple()
{
	uint8_t BrightAmount = 3;
	for( int pos = 0; pos < NUM_LEDS; pos++) { 
		if (directionFlags[pos] == CENTER)
		{
			directionFlags[pos - 1] = LEFT;
			stepFlags[pos - 1] = 5;
			directionFlags[pos + 1] = RIGHT;
			stepFlags[pos +1 ] = 5;
			directionFlags[pos] = FADEUP;
		}
		if (directionFlags[pos] == LEFT) {
			if (stepFlags[pos] > 1) {
				directionFlags[pos - 1] = LEFT;
				stepFlags[pos - 1] = stepFlags[pos] - 1; 
			}
			directionFlags[pos] = FADEUP;
			stepFlags[pos] = BrightAmount;

		}
		if (directionFlags[pos] == RIGHT) {
			if (stepFlags[pos] > 1) {
				directionFlags[pos + 1] = RIGHT;
				stepFlags[pos + 1] = stepFlags[pos] - 1; 
			}
			directionFlags[pos] = FADEUP;
			stepFlags[pos] = BrightAmount;
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
		directionFlags[center] = CENTER;
		stepFlags[center] = RIPPLE_STEPS;
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

void ShiftLedsUp(int end, int start)
{
	for (int pos = start; pos > end; pos--)
	{
		leds[pos] = leds[pos - 1];
	}
}
// 104, 78
void ShiftLedsAround(uint8_t end, uint8_t start)
{
	ledsOriginal[start] = leds[start];
	for (uint8_t pos = start; pos > end; pos--)
	{
		leds[pos] = leds[pos - 1];
	}
	leds[end] = ledsOriginal[start];
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
		leds[MENU_LED_3] = CRGB ( 255, 255, 255 );
		leds[MENU_LED_5] = CRGB ( 255, 255, 255 );	
		break;
		case 3:
		//Heartbeat
		leds[MENU_LED_1] = CRGB ( 255, 0, 0 );
		leds[MENU_LED_3] = CRGB ( 255, 0, 0 );
		leds[MENU_LED_5] = CRGB ( 255, 0, 0 );
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
		case 6 ... 13:
		leds[MENU_LED_1] = MenuColors[cycle - 6];
		leds[MENU_LED_2] = MenuColors[cycle - 6];
		leds[MENU_LED_3] = MenuColors[cycle - 6];
		break;
		
		default:
		break;		
	}
	leds[MENU_LED_1 + NUM_LEDS_PER_STRIP] = leds[MENU_LED_1];
	leds[MENU_LED_2 + NUM_LEDS_PER_STRIP] = leds[MENU_LED_2];
	leds[MENU_LED_3 + NUM_LEDS_PER_STRIP] = leds[MENU_LED_3];
	leds[MENU_LED_4 + NUM_LEDS_PER_STRIP] = leds[MENU_LED_4];
	leds[MENU_LED_5 + NUM_LEDS_PER_STRIP] = leds[MENU_LED_5];
}


void setRandomPixelToOriginalColor()
{
	int pos = random16(NUM_LEDS);
	leds[pos] = ledsOriginal[pos];
}

void ScreenSaver()
{
	EVERY_N_SECONDS( random8(1, 10) ) { 
		AddRipple(random8(NUM_LEDS));
	}
	EVERY_N_MILLISECONDS(random8(30)) { 
		ProcessSparkle(CHSV(random8(145,175), 255, random8(0,150)));	
	}
}

void BiertjeProgram(uint16_t programCounter)
{   
	uint16_t index = programCounter / 6;
	if (programCounter % 6 == 0)
	{
		
		if (index < NUM_LEDS_PER_STRIP)
		{
			ShiftLedsUp(0, NUM_LEDS_PER_STRIP * 2);
			
			leds[0] = CRGB ( 255, 159, 0 );
			leds[NUM_LEDS_PER_STRIP] = CRGB ( 255, 159, 0 );

			uint8_t foam = (index / 100.0) * 30;

			for (int i = 0; i < foam; i++) 
			{
				leds[index - i] = CRGB::White;
				leds[index + NUM_LEDS_PER_STRIP - i] = CRGB::White;
			}
		} 
		if (index >= NUM_LEDS_PER_STRIP && index < 2 * NUM_LEDS_PER_STRIP)
		{
			ShiftLedsUp(2 * NUM_LEDS_PER_STRIP, 3 * NUM_LEDS_PER_STRIP);
			leds[2 * NUM_LEDS_PER_STRIP] = CRGB ( 255, 159, 0 );
			uint8_t foam = ((index - NUM_LEDS_PER_STRIP) / 100.0) * 30;

			for (int i = 0; i < foam; i++) 
			{
				leds[NUM_LEDS_PER_STRIP + index - i] = CRGB::White;
			}
		}
		if (index == 2*NUM_LEDS_PER_STRIP)
		{
			for (int j=0; j<15;j++){
				ShiftLedsUp(3 * NUM_LEDS_PER_STRIP, NUM_LEDS);
		
				leds[3 * NUM_LEDS_PER_STRIP] = CRGB ( 255, 159, 0 );
			
				uint8_t foam = ((index + j- 2*NUM_LEDS_PER_STRIP) / 100.0) * 30;

				for (int i = 0; i < foam; i++) 
				{
					leds[NUM_LEDS_PER_STRIP + index + j - i] = CRGB::White;
				}
			}
		}
	}
	if (index> 2*NUM_LEDS_PER_STRIP){
			ShiftLedsAround(3 * NUM_LEDS_PER_STRIP, NUM_LEDS - 1);
			// EVERY_N_MILLISECONDS( random16(500, 2000) ) { 
			// 	AddRipple(random8(3*NUM_LEDS_PER_STRIP));
			// }
			// //ProcessRipple();	
	}
}	

void SolidColorProgram(CHSV color, uint8_t program_index,uint16_t programCounter)
{
	switch (program_index)
	{
	case 0:
		EVERY_N_MILLISECONDS( random16(100, 800) ) { 
				AddRipple(random8(NUM_LEDS));
		}
		EVERY_N_MILLISECONDS(random8(30)) { 
			ProcessSparkle(MenuColors[random8(7)]);	
		}
		break;
	case 1:
		EVERY_N_MILLISECONDS(30) {
			if (leds[0])
			{
				FillAllLedsSolid(CRGB::Black);
				
			} else
			{
				FillAllLedsSolid(CRGB::White);
				
			}
		}
		
	break;
	default:
		if (programCounter < 5) FillAllLedsSolid(color);
		else {
			
			// EVERY_N_MILLISECONDS(10)
			// {
			// 	ProcessColorSparkle(color);
			// 	//ShiftLedsAround(0, NUM_LEDS);
			// }
		}
		break;
	}
}
