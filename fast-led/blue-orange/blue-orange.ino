#include <FastLED.h>

// How many leds in your strip?
#define SMALLSTRIP 10
#define LONGSTRIP 19
// #define NUM_LEDS (LONGSTRIP * 4)
#define NUM_LEDS 8

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 23
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
	Serial.begin(57600);
	Serial.println("resetting");
	LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
	LEDS.setBrightness(100);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

int pixelColor[] = {255, 32, 0};
int colors[][3] = {
  {255, 32, 0},
  {12, 151, 234},
};

void loop() { 
	static uint8_t hue = 0;
	Serial.print("x");
	// First slide the led in one direction
	// for(int i = 0; i < NUM_LEDS / 2; i++) {
  for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to orange 
		leds[i] = CRGB(32, 255, 0);
   delay(5);
		// Show the leds
		FastLED.show(); 
	}

  delay(100);
	// Now go in the other direction.  
 for(int i = NUM_LEDS; i < NUM_LEDS; i++) {
	// for(int i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
		// Set the i'th led to red 
		leds[i] = CRGB(151, 12, 234);
   delay(5);
		// Show the leds
		FastLED.show();
	}
}
