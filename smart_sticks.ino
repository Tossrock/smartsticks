#include "FastLED.h"
#include "input.h"

#include <Snooze.h>
SnoozeDigital digital;
SnoozeBlock config(digital);

FASTLED_USING_NAMESPACE

#define DATA_PIN  23
#define CLK_PIN   2
#define SWITCH_PIN 4
#define LED_TYPE    APA102
#define COLOR_ORDER BGR
#define NUM_LEDS    16
CRGB leds[32];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

bool systemrunning = true;
byte increment = 1;

const uint8_t PROGMEM pixels_nyan[] = {
0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,
0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,
0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,
0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,
0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0XFF, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0XFF, 0XFF, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,
0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,
0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,
0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,
0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,
0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,
0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,
0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,
0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0XFF, 0XFF, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,
0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0XFF, 0XFF, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0XFF, 0XFF, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,
0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,
0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  0XFF, 0X00, 0XDE,  0XFF, 0X00, 0XDE,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0X00, 0X00,  0X00, 0XF6, 0XFF,  0X00, 0XF6, 0XFF,  };
void setup() {
  delay(3000); // 3 second delay for recovery
  reset_button_state();
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, 32).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  digital.pinMode(SWITCH_PIN, INPUT_PULLUP, FALLING);
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { image_pattern, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, fade };
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
int bright = 4*96;

void loop() {
  byte input = get_input();
  if (bstate.longpressing){
    bright = (bright-1)%1024;
    FastLED.setBrightness(bright/4);
  }
  switch(input){
    case NO_INPUT:
      if (systemrunning) {
        run_pattern();
      } else {
        delay(100);
      }
      break;
    case longpress:
      //bright = (bright - 24)%255;
      //FastLED.setBrightness(bright);
      break;
    case singleclick:
      if (!systemrunning){
        // Wake on single click
        turn_on();
      } else {
        nextPattern();
      }
      break;
    case doubleclick:
      if (increment){
        increment = 0;
      } else {
        increment = 1;
      }
      break;
    case tripleclick:
      //turn on/off
      if (systemrunning){
        turn_off();
      } else {
        turn_on();
      }

      break;
  }
}

void turn_on(){
  systemrunning = true;
  FastLED.setBrightness(bright);
}

void turn_off(){
  systemrunning = false;
  for (byte i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(0,0,0);
    FastLED.setBrightness(0);
    FastLED.clear();
    FastLED.show();
  }
  Snooze.sleep( config );
}

void run_pattern(){
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
  // send the 'leds' array out to the actual LED strip
  for (int i=16; i < 32; i++){
    leds[i] = leds[31-i];
  }
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue+=increment; } // slowly cycle the "base color" through the rainbow
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

int frame = 0;
void image_pattern(){
  int frame_offset = 16 * 3 * frame;
  for (int i=0; i<16; i++){
    byte r = pixels_nyan[frame_offset + 3*i];
    byte g = pixels_nyan[frame_offset + 3*i+1];
    byte b = pixels_nyan[frame_offset + 3*i+2];
    leds[i] = CRGB(r,g,b);
  }
  frame = (frame + 1) % (sizeof(pixels_nyan) / (3*16));
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void fade()
{
  for (int i =0; i < NUM_LEDS; i++){
    leds[i] = CHSV(gHue, 255,255);
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
