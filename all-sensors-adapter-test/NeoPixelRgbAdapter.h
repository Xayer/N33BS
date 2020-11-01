
#ifndef NEO_PIXEL_RGB_ADAPTER_H_
#define NEO_PIXEL_RGB_ADAPTER_H_

#include "Adafruit_NeoPixel.h"
#include "rgbInterface.h"

class NeopixelAdapter: public rgbInterface {
private:
    Adafruit_NeoPixel* neopixel;
public:
    NeopixelAdapter(int amountOfLeds, int pin = 3);
    void setPixel(int led, int red, int green, int blue) override;
    void updatePixels() override;
    void clearPixels() override;
};

#endif