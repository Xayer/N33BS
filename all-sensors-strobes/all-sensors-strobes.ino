/*****************************************************************************/
/*INCLUDES                                                                   */
/*****************************************************************************/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/*****************************************************************************/
/*Constants                                                                  */
/*****************************************************************************/
#define PIN 3
#define SMALLSTRIP 10
#define LONGSTRIP 19
#define NUMPIXELS (SMALLSTRIP + LONGSTRIP * 3)
#define arraySize(x)       (sizeof(x) / sizeof(x[0]))

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/*****************************************************************************/
/*SETUP (Initialisation)                                                          */
/*****************************************************************************/
void setup()
{
    /* Serial setup for UART debugging */
    Serial.begin(115200);
    pixels.begin();
    pixels.clear();
}

/*****************************************************************************/
/*LOOP (runtime super loop)                                                          */
/*****************************************************************************/
int pixelColor[] = {255, 32, 0};
int colors[][3] = {
  {255, 32, 0},
  {12, 151, 234},
};
void loop() {
    for(int colorIndex = 0; colorIndex < arraySize(colors); colorIndex++) {
      pixelColor[0] = colors[colorIndex][0];
      pixelColor[1] = colors[colorIndex][1];
      pixelColor[2] = colors[colorIndex][2];
      drawReverseLine(0);
      fadeOut(0);
      drawLine(0);
      fadeOut(0);
      ripple(NUMPIXELS, 0);
      fadeOut(0);
    }
}
//TOOD: Add a start length so that you can pick where it should start from
void ripple(int striplength, int delayBetweenLeds) {
  // get the center pixel. if ledstrip has even amount of leds, just take the two in the middle.
  int middleBack = floor(striplength / 2)-1; // we start at one
  int middleFront = floor(striplength / 2)+1;
  Serial.printf("front: %d, back: %d\n", middleFront, middleBack);
  if (striplength % 2 > 0) { // if uneven number
    Serial.println(floor(striplength / 2));
    pixels.setPixelColor(floor(striplength / 2), pixels.Color(pixelColor[0], pixelColor[1], pixelColor[2]));
    pixels.show();
    pixels.clear();
  }
  // from center and forward
  for(int i=0; i<floor(striplength / 2); i++) { // Move forward from center
    pixels.setPixelColor(i+middleFront, pixels.Color(pixelColor[0], pixelColor[1], pixelColor[2]));
    pixels.setPixelColor(middleBack-i, pixels.Color(pixelColor[0], pixelColor[1], pixelColor[2]));
    // turn off the "previous led"
    pixels.setPixelColor(i+middleFront-1, pixels.Color(pixelColor[0] / 2, pixelColor[1]  / 2, pixelColor[2] / 2));
    pixels.setPixelColor(middleBack-i+1, pixels.Color(pixelColor[0] / 2, pixelColor[1]  / 2, pixelColor[2] / 2));
    delay(delayBetweenLeds);
    pixels.show();
  }
}

void drawLine(int fadeDelay) {
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(pixelColor[0], pixelColor[1], pixelColor[2]));
    pixels.show();
    delay(fadeDelay);
  }
}

void drawReverseLine(int fadeDelay) {
  for(int i=NUMPIXELS; i>0; i--) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(pixelColor[0], pixelColor[1], pixelColor[2]));
    pixels.show();
    delay(fadeDelay);
  }
}

void fadeOut(int fadeDelay) {
    for(int fadeMultiplier=100; fadeMultiplier>=0; fadeMultiplier--) {
      double fadeStep = 0.01 * fadeMultiplier;
      for(int i=0; i<NUMPIXELS; i++) {
         pixels.setPixelColor(i, pixels.Color(pixelColor[0] * fadeStep, pixelColor[1] * fadeStep, pixelColor[2] * fadeStep));
      }
      pixels.show();
      delay(fadeDelay);
    }
}

void fadeIn(int fadeDelay) {
    for(int fadeMultiplier=0; fadeMultiplier<=100; fadeMultiplier++) {
      double fadeStep = 0.01 * fadeMultiplier;
      for(int i=0; i<NUMPIXELS; i++) {
         pixels.setPixelColor(i, pixels.Color(pixelColor[0] * fadeStep, pixelColor[1] * fadeStep, pixelColor[2] * fadeStep));
      }
      pixels.show();
      delay(fadeDelay);
    }
}
