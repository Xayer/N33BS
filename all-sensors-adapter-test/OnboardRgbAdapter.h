
#ifndef ONBOARD_RGB_ADAPTER_H_
#define ONBOARD_RGB_ADAPTER_H_

#include "rgbInterface.h"

class OnboardAdapter: public rgbInterface {
private:
    int redPort = 22;
    int bluePort = 24;
    int greenPort = 23;

    int redValue = 0;
    int greenValue = 0;
    int blueValue = 0;

public:
    OnboardAdapter(int amountOfLeds = 1, int pin = 25);
    void setPixel(int led, int red, int green, int blue) override;
    void updatePixels() override;
    void clearPixels() override;
};

#endif
