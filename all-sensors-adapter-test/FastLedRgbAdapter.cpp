#include "FastLedRgbAdapter.h"

FastLedRgbAdapter::FastLedRgbAdapter(int amountOfLeds, int pin) {
    CRGB leds[amountOfLeds];
    this->leds = leds;
    this->fastLed = FastLED.addLeds<NEOPIXEL, pin, RGB>(this->leds, amountOfLeds);
    this->fastLed->begin();
}
void FastLedRgbAdapter::setPixel(int led, int red, int green, int blue) {
    this->leds[led].setRGB(red, green, blue);
}
void FastLedRgbAdapter::updatePixels() {
    this->fastLed->show();
}
void FastLedRgbAdapter::clearPixels() {
    this->fastLed->clear();
}
