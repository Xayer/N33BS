#include "Adafruit_NeoPixel.h"
#include "rgbInterface.h"

class NeopixelAdapter: public rgbInterface {
    Adafruit_NeoPixel neopixel;
    public:
        NeopixelAdapter(int amountOfLeds, int pin = 3) {
            this->neopixel = Adafruit_NeoPixel(amountOfLeds, pin, NEO_GRB + NEO_KHZ800);
            this->neopixel.begin();
        }
        void setPixel(int led, int red, int green, int blue) {
            this->neopixel.setPixelColor(led, red, green, blue);
        }
        void updatePixels() {
            this->neopixel.show();
        }
        void clearPixels() {
            this->neopixel.clear();
        }
};
