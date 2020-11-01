#ifndef RGB_STRIP_CONTROLLER_H_
#define RGB_STRIP_CONTROLLER_H_

#include "NeoPixelRgbAdapter.h"
//#include "FastLedRgbAdapter.h"
#include "rgbInterface.h"

class RgbStripController {
    private:
        rgbInterface* adapter;

    public:
        RgbStripController(int amountOfLeds, int pin = 3, char type = 'neo') {
            this->adapter = new NeopixelAdapter(amountOfLeds, pin);
            //this->adapter = new FastLedRgbAdapter(amountOfLeds, pin);
        }
        void setPixel(int led, int red, int green, int blue) {
            this->adapter->setPixel(led, red, green, blue);
        }
        void updatePixels() {
            this->adapter->updatePixels();
        }
        void clearPixels() {
            this->adapter->clearPixels();
        }
};

#endif
