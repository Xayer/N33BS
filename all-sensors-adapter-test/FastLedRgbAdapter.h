
#ifndef FAST_LED_RGB_ADAPTER_H_
#define FAST_LED_RGB_ADAPTER_H_

#include "FastLED.h"
#include "rgbInterface.h"

class FastLedRgbAdapter: public rgbInterface {
private:
    FastLED* fastLed;
    CRGB* leds;
public:
    FastLedRgbAdapter(int amountOfLeds, int pin = 3);
    void setPixel(int led, int red, int green, int blue) override;
    void updatePixels() override;
    void clearPixels() override;
};

#endif
