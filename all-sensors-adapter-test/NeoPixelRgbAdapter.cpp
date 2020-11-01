#include "NeoPixelRgbAdapter.h"

NeopixelAdapter::NeopixelAdapter(int amountOfLeds, int pin) {
    this->neopixel = new Adafruit_NeoPixel(amountOfLeds, pin, NEO_GRB + NEO_KHZ800);
    this->neopixel->begin();
}
void NeopixelAdapter::setPixel(int led, int red, int green, int blue) {
    this->neopixel->setPixelColor(led, red, green, blue);
}
void NeopixelAdapter::updatePixels() {
    this->neopixel->show();
}
void NeopixelAdapter::clearPixels() {
    this->neopixel->clear();
}
