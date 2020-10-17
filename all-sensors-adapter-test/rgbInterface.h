#ifndef RGBINTERFACE_H_
#define RGBINTERFACE_H_

class rgbInterface
{
public:
	int pin;
	int amountOfLeds;

	/* Used for required setup */
	rgbInterface() {}
	rgbInterface(int amountOfLeds, int pin = 3) {
		this->amountOfLeds = amountOfLeds;
		this->pin = pin;
	}
	/* Set a Pixel's RGB value */
	virtual void setPixel(int led, int red, int green, int blue) {};
	/* Used to show the updates that were applied using setPixel */
	virtual void updatePixels() {};
	/* Clear all LEDs */
	virtual void clearPixels() {};
};

#endif