/*
  Nano33BLESensorExample_proximity.ino
  Copyright (c) 2020 Dale Giancono. All rights reserved..

  This program is an example program showing some of the cababilities of the 
  Nano33BLESensor Library. In this case it outputs proximity data from one of 
  the Arduino Nano 33 BLE Sense's on board sensors via serial in a format that 
  can be displayed on the Arduino IDE serial plotter. It also outputs the data 
  via BLE in a string format that can be viewed using a variety of BLE scanning 
  software.

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
/* For the bluetooth funcionality */
#include <ArduinoBLE.h>
#include "Nano33BLEProximity.h"

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
/* 
 * We use strings to transmit the data via BLE, and this defines the buffer
 * size used to transmit these strings. Only 20 bytes of data can be 
 * transmitted in one packet with BLE, so a size of 20 is chosen the the data 
 * can be displayed nicely in whatever application we are using to monitor the
 * data.
 */
#define BLE_BUFFER_SIZES             20
/* Device name which can be scene in BLE scanning software. */
#define BLE_DEVICE_NAME                "Fake PoweredUp Hub"
/* Local name which should pop up when scanning for BLE devices. */
#define BLE_LOCAL_NAME                "HUB NO.4"

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/
/* 
 * Nano33BLEProximityData object which we will store data in each time we read 
 * the proximity data. 
 */ 
Nano33BLEProximityData proximityData;

/* 
 * Declares the BLEService and characteristics we will need for the BLE 
 * transfer. The UUID was randomly generated using one of the many online 
 * tools that exist. It was chosen to use BLECharacteristic instead of 
 * BLEIntCharacteristic was it is hard to view int data in most BLE 
 * scanning software. Strings can be viewed easiler enough. In an actual 
 * application you might want to transfer ints directly.
 */
BLEService BLESensors("00001623-1212-efde-1623-785feabcd123");
BLECharacteristic LegoServiceCharacteristic("00001624-1212-efde-1623-785feabcd123", BLERead | BLEWrite | BLENotify | BLEWriteWithoutResponse | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic wedoDistanceSensor("1560", BLERead | BLENotify | BLEWriteWithoutResponse | BLEBroadcast, BLE_BUFFER_SIZES);

/* Common global buffer will be used to write to the BLE characteristics. */
char bleBuffer[BLE_BUFFER_SIZES];
byte connected = 0;
long long unsigned int portTypeBuffer[20] = { 0x0F, 0x00, 0x04, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00, 0x00 };
/*****************************************************************************/
/*SETUP (Initialisation)                                                     */
/*****************************************************************************/
void setup()
{
    /* 
     * Serial setup. This will be used to transmit data for viewing on serial 
     * plotter 
     */
    Serial.begin(115200);


    /* BLE Setup. For information, search for the many ArduinoBLE examples.*/
    if (!BLE.begin()) 
    {
        while (1);    
    }
    else
    {
        BLE.setDeviceName(BLE_DEVICE_NAME);
        BLE.setLocalName(BLE_LOCAL_NAME);
        BLE.setAdvertisedService(BLESensors);
        //BLESensors.addCharacteristic(wedoDistanceSensor);
        BLESensors.addCharacteristic(LegoServiceCharacteristic);

        BLE.addService(BLESensors);
        BLE.advertise();
        /* 
         * Initialises the proximity sensor, and starts the 
         * periodic reading of the sensor using a Mbed OS thread. 
         * The data is placed in a circular buffer and can be read whenever.
         */
        Proximity.begin();

        /* Plots the legend on Serial Plotter */
        Serial.println("Proximity\r\n");
    }
}

/*****************************************************************************/
/*LOOP (runtime super loop)                                                  */
/*****************************************************************************/
void loop()
{
    BLEDevice central = BLE.central();
    if(central)
    {
        /* 
         * If a BLE device is connected, the data will start being read, 
         * and the data will be written to each BLE characteristic. The same 
         * data will also be output through serial so it can be plotted using 
         * Serial Plotter. 
         */
        if(central.connected())
        {    
            if (!connected) {
              //uint8_t portTypeBuffer[] = { 0x05, 0x00, 0x04, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              //0x00, 0x00, 0x00, 0x00, 0x00 };
                uint8_t portTypeBuffer[] = { 0x0F, 0x00, 0x04, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00, 0x00 };
              // data with possibly engine running
              // uint8_t portTypeBuffer[] = { 0x05, 0x00, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
              //0x00, 0x00, 0x00, 0x00, 0x00 };
              LegoServiceCharacteristic.writeValue(portTypeBuffer, sizeof portTypeBuffer);
              connected = 1;
            }
            //if (LegoServiceCharacteristic.valueUpdated()) {
              // watch if the characteristic has changed, if it has modify our local copy to compare with.
              //Serial.println("value: \n");
              //portTypeBuffer = LegoServiceCharacteristic.value();
              uint8_t valueBuffer[20] = {};
              LegoServiceCharacteristic.readValue(valueBuffer, 20);
              int i=0;
              
              while (i < sizeof(valueBuffer))
              {
                   Serial.println(valueBuffer[i], HEX);
                   i++;
              }
              Serial.println("\n-----\n");
              delay(1000);
            //}
            
            /* 
             * sprintf is used to convert the read float value to a string 
             * which is stored in bleBuffer. This string is then written to 
             * the BLE characteristic. 
             */
//            if(Proximity.pop(proximityData))
//            {
//                writeLength = sprintf(bleBuffer, "%d", proximityData.proximity);
//                wedoDistanceSensor.writeValue(bleBuffer, writeLength); 
//                
//                Serial.printf("%d\r\n", proximityData.proximity);
//            }

        }
    }
}
