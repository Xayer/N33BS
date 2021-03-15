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
#define X1_0 0
#define X1_1 1

#define X2_0 7
#define X2_1 8

#define X3_0 5
#define X3_1 6

#define X4_0 11
#define X4_1 12

// Optocuplers
#define OC1_0 10 // PIN_POWER_FEEDBACK
#define OC1_1 16 // PIN_POWER_SWITCH
#define OC2_0 15 // PIN_PUSH_BUTTON_PIN
#define OC2_1 14 // PIN_CHARGING_RELAY

#define PIN_PUSH_BUTTON_LED 17
#define PIN_TOP_IGNITION_RELAY 21
#define PIN_SHUT_DOWN_RELAY 20
#define PIN_LED_DATA 23
#define PIN_VOLTAGE_DIVIDER A8

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    10
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  240

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

const int ports[] = {
  X1_0,
  X1_1,
  X2_0,
  X2_1,
  X3_0,
  X3_1,
  X4_0,
  X4_1,
  OC1_0,
  OC1_1,
  OC2_0,
  OC2_1,
  PIN_PUSH_BUTTON_LED,
  PIN_TOP_IGNITION_RELAY,
  PIN_SHUT_DOWN_RELAY,
  PIN_LED_DATA,
};

void setup() {
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,PIN_LED_DATA,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  
  Serial.begin(9600);
  
  for(int portIndex = 0; ARRAY_SIZE(ports) >= portIndex; portIndex++) {
    pinMode(ports[portIndex], OUTPUT);
    Serial.printf("\n%d (index: %d)", ports[portIndex], portIndex);
  }
}
uint8_t gHue = 100; // rotating "base color" used by many of the patterns

uint8_t lastChangedPort = -1;
String readString;

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  confetti();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  // FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  // EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  // EVERY_N_SECONDS( 3 ) { confetti(); } // change patterns periodically

  while (Serial.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = Serial.read();
    readString += c;
  }

  if (readString.length() >0) {
    toggleItem(readString.toInt());

    readString = "";
  }
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
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
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
