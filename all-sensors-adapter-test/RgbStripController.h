#ifndef RGB_STRIP_CONTROLLER_H_
#define RGB_STRIP_CONTROLLER_H_

#include "NeoPixelRgbAdapter.h"
#include "OnboardRgbAdapter.h"
#include "rgbInterface.h"

class RgbStripController {
private:
    rgbInterface* adapter;

public:
    RgbStripController(int amountOfLeds, int pin = 3, char type = 'n') {
        switch(type) {
          case 'n':
            this->adapter = new NeopixelAdapter(amountOfLeds, pin);
            break;
          case 'o':
            this->adapter = new OnboardAdapter(amountOfLeds, pin);
            break;
          default:
            this->adapter = new NeopixelAdapter(amountOfLeds, pin);
            break;
        }
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
=======
#ifndef RGB_STRIP_CONTROLLER_H_
#define RGB_STRIP_CONTROLLER_H_

#include "NeoPixelRgbAdapter.h"
#include "OnboardRgbAdapter.h"
#include "rgbInterface.h"

class RgbStripController {
    private:
        rgbInterface* adapter;

public:
    RgbStripController(int amountOfLeds, int pin = 3, char type = 'n') {
        switch(type) {
          case 'n':
            this->adapter = new NeopixelAdapter(amountOfLeds, pin);
            break;
          case 'o':
            this->adapter = new OnboardAdapter(amountOfLeds, pin);
            break;
          default:
            this->adapter = new NeopixelAdapter(amountOfLeds, pin);
            break;
        }
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
