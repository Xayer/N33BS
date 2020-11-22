/*****************************************************************************/
/*INCLUDES                                                                   */
/*****************************************************************************/
#include "RgbStripController.h"
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

RgbStripController* pixels;

/*****************************************************************************/
/*SETUP (Initialisation)                                                          */
/*****************************************************************************/
void setup()
{
    /* Serial setup for UART debugging */
    Serial.begin(115200);

    pixels = new RgbStripController(NUMPIXELS, PIN, 'o');
    pixels->clearPixels();
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
    pixels->setPixel(0, 255, 0, 0);
    pixels->updatePixels();
    delay(1000);
}
