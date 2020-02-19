
#include "FastLED.h"
void drawTwinkles( CRGBSet& L);
CRGB computeOneTwinkle( uint32_t ms, uint8_t salt);
uint8_t attackDecayWave8( uint8_t i);
void coolLikeIncandescent( CRGB& c, uint8_t phase);