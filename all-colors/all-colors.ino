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
    for(int led=0; led<NUMPIXELS; led++) {
      int red = random(0,50);
      int green = random(0,50);
      int blue = random(0,50);
      Serial.printf("red:%d, green:%d, blue:%d", red, green, blue);
      pixels.setPixelColor(led, pixels.Color(red, green, blue));
    }
    pixels.show();
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
