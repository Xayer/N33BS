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
int counter = 0;
byte subscribed = 0;

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
        BLESensors.addCharacteristic(LegoServiceCharacteristic);

        BLE.addService(BLESensors);
        BLE.setAdvertisingInterval(640);
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
        while(central.connected())
        {
            if (LegoServiceCharacteristic.written()) {
              Serial.println("value was updated!");
            }
            if (!connected) {
              LegoServiceCharacteristic.setEventHandler(BLEWritten, dataUpdated);
              connected = 1;
            }
            
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

//void writeMessage(byte[] message, bool addLength = true) {
//  int length = addLength ? sizeof message + 2 : sizeof message;
//  LegoServiceCharacteristic.writeValue(message, length);
//}

void dataUpdated(BLEDevice central, BLECharacteristic characteristic) {
  
  // central wrote new value to characteristic, update LED
  //Serial.print("Characteristic event, written: ");

  uint8_t currentValueBuffer[LegoServiceCharacteristic.valueLength()] = {};
  LegoServiceCharacteristic.readValue(currentValueBuffer, sizeof currentValueBuffer);
  // Output the message before changing:
  int i=0;

  Serial.printf("Recieved State with a size of %d: %d",sizeof currentValueBuffer, counter);
  while (i < sizeof(currentValueBuffer))
  {
       Serial.println(currentValueBuffer[i], HEX);
       i++;
  }
  Serial.println("-----");

  if (currentValueBuffer[2] == 0x04) {
    Serial.println("--- I/O info requested ---");
    // we say that we have something in port A or 1
    uint8_t attachedIoResponse[15] = { 0x15, 0x00, 0x04, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00 };
    LegoServiceCharacteristic.writeValue(attachedIoResponse, sizeof attachedIoResponse);
    // 
    uint8_t attachedPortResonse[14] = { 0x14, 0x00, 0x04, 59, 0x01, 21, 0x00, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 };
    LegoServiceCharacteristic.writeValue(attachedPortResonse, sizeof attachedPortResonse);
    //
    uint8_t attachedPortSecondResonse[14] = { 0x14, 0x00, 0x04, 60, 0x01, 20, 0x00, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 };
    LegoServiceCharacteristic.writeValue(attachedPortSecondResonse, sizeof attachedPortSecondResonse);
  }
  
  // Hub Properties
  if (currentValueBuffer[2] == 1) {
    Serial.println("--- Hub Props ---");
    // Requesting for button press
    if (currentValueBuffer[3] == 2) {
      Serial.println("--- button press requested ---");
      uint8_t buttonPressResponse[6] = { 0x06, 0x01, 0x02, 0x02, 0x01 };
      LegoServiceCharacteristic.writeValue(buttonPressResponse, sizeof buttonPressResponse);
      return;
    }
    if (currentValueBuffer[3] == 3) {
      Serial.println("--- firmware ---");
      // requesting firmware version.
      uint8_t firmwareResponse[9] = { 9, 0x01, 0x03, 0x06, 0x00, 0x00, 0x02, 0x11 };
      LegoServiceCharacteristic.writeValue(firmwareResponse, sizeof firmwareResponse);
      return;
    }
    
    // Battery Voltage
    else if (currentValueBuffer[3] == 0x06) {
      Serial.println("--- Battery Voltage ---");
      uint8_t batteryPercentage[5] = { 0x00, 0x00, 0x01, 0x06, 0x06, 31 };
      LegoServiceCharacteristic.writeValue(batteryPercentage, sizeof batteryPercentage);
      return;
    }
  }

  /*if (currentValueBuffer[2] == 0x51) {
    // all responses to 0x81 must be sent over 0x82
    if (currentValueBuffer[3] == 0x20) {
      if (currentValueBuffer[4] == 0x11) {
        Serial.println("---modify LED---");
        // https://lego.github.io/lego-ble-wireless-protocol-docs/index.html#output-sub-command-setrgbcolorno-colorno-n-a
        if (currentValueBuffer[5] == 0x34) {
          int colorIndex = (unsigned char)(currentValueBuffer[7]);
          char colors[10][10] = {"black", "pink", "blue", "lblue", "cyan", "green", "yellow", "orange", "red", "white" };
          Serial.printf("SET RGB to color index (%s)", colors[colorIndex-1]);
          // we must tell that we are busy before we can say that we are done
          uint8_t portInProgressResponse[20] = { 0x14, 0x00, 0x82, 0x32, 0x01, 0x51, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00 };
          LegoServiceCharacteristic.writeValue(portInProgressResponse, sizeof portInProgressResponse);
          delay(500);
          // tell that we are finished
          uint8_t colorUpdateCompleteResponse[20] = { 0x14, 0x00, 0x82, 0x32, 0x01, 0x51, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00 };
          LegoServiceCharacteristic.writeValue(colorUpdateCompleteResponse, sizeof colorUpdateCompleteResponse);
          delay(500);
          // tell that we are idle
          uint8_t portIdleResponse[20] = { 0x14, 0x00, 0x82, 0x32, 0x01, 0x51, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00 };
          LegoServiceCharacteristic.writeValue(portIdleResponse, sizeof portIdleResponse);
          return;
        }
      }
    }
  }*/
}
