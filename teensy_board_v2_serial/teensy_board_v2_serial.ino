#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
// #warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

// I/O Ports
#define X1_IN 0
#define X1_OUT 1

#define X2_IN 7
#define X2_OUT 8

#define X3_IN 6
#define X3_OUT 5

#define X4_IN 12
#define X4_OUT 11

// Optocuplers
#define OC1_IN 10 // PIN_POWER_FEEDBACK
#define OC1_OUT 16 // PIN_POWER_SWITCH
#define OC2_IN 15 // PIN_PUSH_BUTTON_PIN
#define OC2_OUT 14 // PIN_CHARGING_RELAY

#define PIN_PUSH_BUTTON_LED 17
#define PIN_TOP_IGNITION_RELAY 21
#define PIN_SHUT_DOWN_RELAY 20
#define PIN_LED_DATA 23
#define PIN_VOLTAGE_DIVIDER A8
#define PIN_OUTPUT_IMU A5

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    BULB
#define BULB        8
#define STRIP       19
CRGB leds[NUM_LEDS];

CRGB X1_LEDS[NUM_LEDS];
CRGB X2_LEDS[NUM_LEDS];
CRGB X3_LEDS[NUM_LEDS];
CRGB X4_LEDS[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  160

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define shouldCycleColors false

const int ports[] = {
  X1_IN,
  X1_OUT,
  X2_IN,
  X2_OUT,
  X3_IN,
  X3_OUT,
  X4_IN,
  X4_OUT,
  OC1_IN,
  OC1_OUT,
  OC2_IN,
  OC1_OUT,
  PIN_PUSH_BUTTON_LED,
  PIN_TOP_IGNITION_RELAY,
  PIN_SHUT_DOWN_RELAY,
  PIN_LED_DATA,
};

void setup() {
  delay(2000);
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, PIN_LED_DATA, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  fadeToBlackBy( leds, NUM_LEDS, 0);
  FastLED.addLeds<LED_TYPE, X1_OUT, COLOR_ORDER>(X1_LEDS, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, X2_OUT, COLOR_ORDER>(X2_LEDS, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, X3_OUT, COLOR_ORDER>(X3_LEDS, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, X4_OUT, COLOR_ORDER>(X4_LEDS, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  // Serial.begin(9600);

  /*for (int portIndex = 0; ARRAY_SIZE(ports) > portIndex; portIndex++) {
    pinMode(ports[portIndex], OUTPUT);
    Serial.printf("Port: %d\n", ports[portIndex]);
  }*/
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, drawLine };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 53; // rotating "base color" used by many of the patterns

uint8_t lastChangedPort = -1;
String readString;

void loop()
{
  if(shouldCycleColors) {
    gHue += 1;
  
    if(gHue == 360) {
      gHue = 0;
    }
  }

  gPatterns[gCurrentPatternNumber]();

  if(Serial.available() > 0) {
    while(Serial.available() > 0) {
      changeEffect();
    }
  }

  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  

  // do some periodic updates
  // EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  // EVERY_N_SECONDS( 3 ) { confetti(); } // change patterns periodically

  /*while (Serial.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = Serial.read();
    readString += c;
  }*/

  /*if (readString.length() > 0) {
    toggleItem(readString.toInt());

    readString = "";
  }*/
}

void changeEffect() {
  int dataIn = Serial.parseInt();

  Serial.println(dataIn);

  if (dataIn > 0) {

    switch(dataIn){
      case 1:
        Serial.println("rainbow");
        gCurrentPatternNumber = 0;
        break;
      case 2:
        Serial.println("rainbowWithGlitter");
        gCurrentPatternNumber = 1;
        break;
      case 3:
        Serial.println("confetti");
        gCurrentPatternNumber = 2;
        break;
      case 4:
        Serial.println("sinelon");
        gCurrentPatternNumber = 3;
        break;
      case 5:
        Serial.println("juggle");
        gCurrentPatternNumber = 4;
        break;
      case 6:
        Serial.println("bpm");
        gCurrentPatternNumber = 5;
        break;
      case 7:
        Serial.println("drawLine");
        gCurrentPatternNumber = 6;
        break;
      default:
        FastLED.setBrightness(dataIn);
    }
  }
}

void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(128), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 250, 0, NUM_LEDS - 1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void toggleItem(int port) {
  //int portValue = digitalRead(ports[itemIndex]);
  int portValue = digitalRead(port);
  Serial.printf("\ntoggling port: %d", port);
  Serial.printf("\nexisting Value: %d", portValue);
  int newValue = portValue ? LOW : HIGH;
  Serial.printf("\nnew value: %d\n", newValue);
  digitalWrite(port, newValue);
}

void drawLine() {
  for(int i=0; i<NUM_LEDS; i++) { // For each pixel...
    leds[i] = CHSV( gHue, 0, 255);
    FastLED.show();
    FastLED.delay(75);
    // FastLED.show();
  }
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

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
