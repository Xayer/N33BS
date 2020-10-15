#include "NeoPixelRgbAdapter.cpp"

class RgbStripController {
    rgbInterface adapter;
    public:
        RgbStripController(int amountOfLeds, int pin = 3) {
            this->adapter = NeopixelAdapter(amountOfLeds, pin);
        }
        void setPixel(int led, int red, int green, int blue) {
            this->adapter.setPixel(led, red, green, blue);
        }
        void updatePixels() {
            this->adapter.updatePixels();
        }
        void clearPixels() {
            this->adapter.clearPixels();
        }
};
