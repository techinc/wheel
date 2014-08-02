#include <FastLED.h>

// Adapted from Cylon, and Fire2012-with-colorpallette

#define DATA_PIN 12
#define CLOCK_PIN 11

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 60  // was 60

#define NUM_LEDS    (5+5+5+5+5+5+5+5)
#define SPOKE_LENGTH 5
#define STARTS_AT(N) (&(leds[N]))

CRGB leds[NUM_LEDS];
CRGB* north=STARTS_AT(0);
CRGB* northeast=STARTS_AT(SPOKE_LENGTH);
CRGB* east=STARTS_AT(2*SPOKE_LENGTH);
CRGB* southeast=STARTS_AT(3*SPOKE_LENGTH);
CRGB* south=STARTS_AT(4*SPOKE_LENGTH);
CRGB* southwest=STARTS_AT(5*SPOKE_LENGTH);
CRGB* west=STARTS_AT(6*SPOKE_LENGTH);
CRGB* northwest=STARTS_AT(7*SPOKE_LENGTH);

CRGBPalette16 gPal;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {
  delay(3000); // sanity delay
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, BRG>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );

  // This first palette is the basic 'black body radiation' colors,
  // which run from black to red to bright yellow to white.
  //gPal = HeatColors_p;
  
  // These are other ways to set up the color palette for the 'fire'.
  // First, a gradient from black to red to yellow to white -- similar to HeatColors_p
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  
  // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
     gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  
  // Third, here's a simpler, three-step gradient, from black to red to white
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);

 
  currentPalette = RainbowColors_p;
  currentBlending = BLEND;

}

void loop()
{
  // Knight rider
  Cylon();

  // Fire fire fire

  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

//  Fire2012WithPalette(north); // run simulation frame, using palette colors
//  Fire2012WithPalette(northeast); // run simulation frame, using palette colors
//  Fire2012WithPalette(east); // run simulation frame, using palette colors
//  Fire2012WithPalette(southeast); // run simulation frame, using palette colors
//  Fire2012WithPalette(south); // run simulation frame, using palette colors
//  Fire2012WithPalette(southwest); // run simulation frame, using palette colors
//  Fire2012WithPalette(west); // run simulation frame, using palette colors
//  Fire2012WithPalette(northwest); // run simulation frame, using palette colors

  // Rainbow glow stuff
//  ChangePalettePeriodically();
//  static uint8_t startIndex = 0;
//  startIndex = startIndex + 1; /* motion speed */
//  FillLEDsFromPaletteColors( startIndex);

  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}


// Fire2012 by Mark Kriegsman, July 2012
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100 
#define COOLING  2

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 10

void Fire2012WithPalette(CRGB* spoke)
{
// Array of temperature readings at each simulation cell
  static byte heat[SPOKE_LENGTH];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < SPOKE_LENGTH; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / SPOKE_LENGTH) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= SPOKE_LENGTH - 3; k > 0; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < SPOKE_LENGTH; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      spoke[j] = ColorFromPalette( gPal, colorindex);
    }
}

void Cylon() { 
	// First slide the led in one direction
	for(int i = 0; i < SPOKE_LENGTH; i++) {
		// Set the i'th led to red 
		north[i] = CRGB::Red;
		northeast[i] = CRGB::Red;
		east[i] = CRGB::Red;
		southeast[i] = CRGB::Red;
		south[i] = CRGB::Red;
		southwest[i] = CRGB::Red;
		west[i] = CRGB::Red;
		northwest[i] = CRGB::Red;
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		north[i] = CRGB::Black;
		northeast[i] = CRGB::Black;
		east[i] = CRGB::Black;
		southeast[i] = CRGB::Black;
		south[i] = CRGB::Black;
		southwest[i] = CRGB::Black;
		west[i] = CRGB::Black;
		northwest[i] = CRGB::Black;
		// Wait a little bit before we loop around and do it again
		delay(100);
	}

	// Now go in the other direction.  
	for(int i = SPOKE_LENGTH-1; i >= 0; i--) {
		// Set the i'th led to red 
		north[i] = CRGB::Red;
		northeast[i] = CRGB::Red;
		east[i] = CRGB::Red;
		southeast[i] = CRGB::Red;
		south[i] = CRGB::Red;
		southwest[i] = CRGB::Red;
		west[i] = CRGB::Red;
		northwest[i] = CRGB::Red;
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		north[i] = CRGB::Black;
		northeast[i] = CRGB::Black;
		east[i] = CRGB::Black;
		southeast[i] = CRGB::Black;
		south[i] = CRGB::Black;
		southwest[i] = CRGB::Black;
		west[i] = CRGB::Black;
		northwest[i] = CRGB::Black;
		// Wait a little bit before we loop around and do it again
		delay(100);
	}
}

// ColorPallete rainbow stuff

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = BLEND; }
    if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
    if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = BLEND; }
    if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = BLEND; }
    if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = BLEND; }
    if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
    if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = BLEND; }
    if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = BLEND; }
    if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = BLEND; }
    if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
    if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = BLEND; }
  }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
  for( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;
  
  currentPalette = CRGBPalette16( 
    green,  green,  black,  black,
    purple, purple, black,  black,
    green,  green,  black,  black,
    purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more 
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

