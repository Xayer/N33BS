#include "OnboardRgbAdapter.h"
#include<Arduino.h>

OnboardAdapter::OnboardAdapter(int amountOfLeds, int pin) {
    pinMode(this->redPort, OUTPUT);
    pinMode(this->greenPort, OUTPUT);
    pinMode(this->bluePort, OUTPUT);
}
void OnboardAdapter::setPixel(int led, int red, int green, int blue) {
    Serial.printf("red: %d", this->redValue);
    this->redValue = red;
    Serial.printf("green: %d", this->redValue);
    this->greenValue = green;
    Serial.printf("blue: %d", this->redValue);
    this->blueValue = blue;
}
void OnboardAdapter::updatePixels() {
    Serial.printf("red: %s", this->redValue > 0 ? "HIGH" : "LOW");
    digitalWrite(this->redPort, this->redValue > 0 ? HIGH : LOW);
    Serial.printf("green: %s", this->greenValue > 0 ? "HIGH" : "LOW");
    digitalWrite(this->greenPort, this->greenValue > 0 ? HIGH : LOW);
    Serial.printf("blue: %s", this->blueValue > 0 ? "HIGH" : "LOW");
    digitalWrite(this->bluePort, this->blueValue > 0 ? HIGH : LOW);
}
void OnboardAdapter::clearPixels() {
    digitalWrite(this->redPort, LOW);
    digitalWrite(this->greenPort, LOW);
    digitalWrite(this->bluePort, LOW);
}
