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
#define FRAMES_PER_SECOND  240

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
  fadeToBlackBy( leds, NUM_LEDS, 2000);
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
  // Call the current pattern function once, updating the 'leds' array
  //sinelon(leds);
  //confetti();
  //sinelonBulb();
  //sinelonX1();
  drawLine(75, leds);
  drawLine(75, X1_LEDS);
  drawLine(75, X2_LEDS);
  drawLine(75, X3_LEDS);
  drawLine(75, X4_LEDS);

  FastLED.show();

  FastLED.delay(75);

  fill_solid ( leds, NUM_LEDS, CRGB::Black);
  fill_solid ( X1_LEDS, NUM_LEDS, CRGB::Black);
  fill_solid ( X2_LEDS, NUM_LEDS, CRGB::Black);
  fill_solid ( X3_LEDS, NUM_LEDS, CRGB::Black);
  fill_solid ( X4_LEDS, NUM_LEDS, CRGB::Black);

  FastLED.show();

  FastLED.delay(25);

  drawLine(75, leds);
  drawLine(75, X1_LEDS);
  drawLine(75, X2_LEDS);
  drawLine(75, X3_LEDS);
  drawLine(75, X4_LEDS);

  FastLED.show();

  FastLED.delay(75);

  fill_solid ( leds, NUM_LEDS, CRGB::Black);
  fill_solid ( X1_LEDS, NUM_LEDS, CRGB::Black);
  fill_solid ( X2_LEDS, NUM_LEDS, CRGB::Black);
  fill_solid ( X3_LEDS, NUM_LEDS, CRGB::Black);
  fill_solid ( X4_LEDS, NUM_LEDS, CRGB::Black);

  FastLED.show();

  FastLED.delay(250);
  
//  drawLine(75, leds);
//  drawLine(10, X1_LEDS);
//  sinelon(X1_LEDS);
//  confetti(X1_LEDS);
//  addGlitter(4, X2_LEDS);
//  confetti(X2_LEDS);
//  drawLine(75, X3_LEDS);
//  confetti(X3_LEDS);
//  drawLine(75, X4_LEDS);
  //addGlitter(10, X2_LEDS);

  // send the 'leds' array out to the actual LED strip
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

void addGlitter( fract8 chanceOfGlitter, CRGB* ledArray)
{
  if ( random8() < chanceOfGlitter) {
    ledArray[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti(CRGB* ledArray)
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( ledArray, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  ledArray[pos] += CHSV( gHue + random8(128), 200, 255);
}

void sinelon(CRGB* ledArray)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( ledArray, NUM_LEDS, 20);
  int pos = beatsin16( 250, 0, NUM_LEDS - 1 );
  ledArray[pos] += CHSV( gHue, 255, 192);
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

void drawLine(int fadeDelay, CRGB* ledArray) {
  for(int i=0; i<NUM_LEDS; i++) { // For each pixel...
    ledArray[i] = CHSV( gHue, 0, 255);
    // FastLED.show();
    // FastLED.delay(fadeDelay);
  }
}
