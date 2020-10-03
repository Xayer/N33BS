/*
  Nano33BLESensorExample_AllSensors-SerialPlotter.ino
  Copyright (c) 2020 Dale Giancono. All rights reserved..

  This program is an example program showing some of the cababilities of the 
  Nano33BLESensor Library. In this case it outputs sensor data and from all 
  of the Arduino Nano 33 BLE Sense's on board sensors via serial in a format 
  that can be displayed on the Arduino IDE serial plotter.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*****************************************************************************/
/*INCLUDES                                                                   */
/*****************************************************************************/
#include "Arduino.h"
#include "Nano33BLEAccelerometer.h"
#include "Nano33BLEGyroscope.h"
#include "Nano33BLEMagnetic.h"
#include "Nano33BLEProximity.h"
#include "Nano33BLEColour.h"
#include "Nano33BLEGesture.h"
#include "Nano33BLEPressure.h"
#include "Nano33BLETemperature.h"
#include "Nano33BLEMicrophoneRMS.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/*****************************************************************************/
/*Constants                                                                  */
/*****************************************************************************/
#define PIN 3
#define NUMPIXELS 10
#define REFRESH_INTERVAL 10000

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/
/* 
 * Objects which we will store data in each time we read
 * the each sensor. 
 */ 
Nano33BLEMagneticData magneticData;
Nano33BLEGyroscopeData gyroscopeData;
Nano33BLEAccelerometerData accelerometerData;
Nano33BLEProximityData proximityData;
Nano33BLEColourData colourData;
Nano33BLEGestureData gestureData;
Nano33BLEPressureData pressureData;
Nano33BLETemperatureData temperatureData;
Nano33BLEMicrophoneRMSData MicrophoneRMSData;

/*****************************************************************************/
/*SETUP (Initialisation)                                                          */
/*****************************************************************************/
void setup()
{
    /* Serial setup for UART debugging */
    Serial.begin(115200);
    /* 
     * Initialises the all the sensor, and starts the periodic reading 
     * of the sensor using a Mbed OS thread. The data is placed in a 
     * circular buffer and can be read whenever.
     */
    Magnetic.begin();
    Gyroscope.begin();
    Accelerometer.begin();
    Proximity.begin();
    Colour.begin();
    Gesture.begin();
    Pressure.begin();
    Temperature.begin();
    MicrophoneRMS.begin();
    pixels.begin();
    delay(500);
}

/*****************************************************************************/
/*LOOP (runtime super loop)                                                          */
/*****************************************************************************/
void loop()
{
    /*
     * This gets all the data from each sensor. Note that each sensor gets data
     * at different frequencies. Seeing as this super loop runs every 50mS, not
     * all the sensors will have new data. If a sensor does not have new data, 
     * the old data will just be printed out again. This is a little sloppy, but
     * allows the data to be printed in a coherrent way inside serial plotter.
     */
    //Magnetic.pop(magneticData);
    //Gyroscope.pop(gyroscopeData);
    //Accelerometer.pop(accelerometerData);
    //Proximity.pop(proximityData);
    //Colour.pop(colourData);
    //Gesture.pop(gestureData);
    //Pressure.pop(pressureData);
    Temperature.pop(temperatureData);
    //MicrophoneRMS.pop(MicrophoneRMSData);

    /*Serial.printf("%f,%f,%f,", magneticData.x, magneticData.y, magneticData.z);
    Serial.printf("%f,%f,%f,", gyroscopeData.x, gyroscopeData.y, gyroscopeData.z);
    Serial.printf("%f,%f,%f,", accelerometerData.x, accelerometerData.y, accelerometerData.z);
    Serial.printf("%d,", proximityData.proximity);
    Serial.printf("%d,%d,%d,%d,", colourData.r, colourData.g, colourData.b, colourData.c);
    Serial.printf("%d,", gestureData.gesture);
    Serial.printf("%d,", gestureData.gesture);
    Serial.printf("%f,", pressureData.barometricPressure);*/
    Serial.printf("\n%f\n", temperatureData.temperatureCelsius);
    /*Serial.printf("%f,%f,", temperatureData.temperatureCelsius, temperatureData.humidity);
    Serial.printf("%d \r\n", MicrophoneRMSData.RMSValue);*/
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();

    int temperature = floor(temperatureData.temperatureCelsius);

    // set first pixel color as an indicator of the 10's
    int colorOfTens[] =  { 255,255,255 };

    switch(temperature) {
      case -10 ... -1 :
          colorOfTens[0] = 255;
          colorOfTens[1] = 255;
          colorOfTens[2] = 255;
        break;
      case 0 ... 19 :
          colorOfTens[0] = 0;
          colorOfTens[1] = 0;
          colorOfTens[2] = 255;
        break;
      case 20 ... 29 :
          colorOfTens[0] = 0;
          colorOfTens[1] = 255;
          colorOfTens[2] = 0;
        break;
      case 30 ... 39 :
          colorOfTens[0] = 255;
          colorOfTens[1] = 0;
          colorOfTens[2] = 0;
        break;
    }
    pixels.setPixelColor(0, pixels.Color(colorOfTens[0], colorOfTens[1], colorOfTens[2]));

    int Ones = temperature % 10;

    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.
    for(int i=1; i< Ones; i++) { // For each pixel...
  
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(colorOfTens[0], colorOfTens[1], colorOfTens[2]));
      delay(250);
      pixels.show();   // Send the updated pixel colors to the hardware.
    }
    delay(REFRESH_INTERVAL);
}
