// SELECT Arduino Duemilanove or Diecimila in Board options
#include "PinChangeInt.h"
#include "PinChangeIntConfig.h"
#include "EEPROM.h"
#include "fuzzy_table.h"
#include "PID_Beta6.h"
#include "MotorWheel.h"
#include "Omni4WD.h"

#include "fuzzy_table.h"
#include "PID_Beta6.h"

// SERIAL LIBRARIES

#include <stdlib.h>
#include <SerialCmd.h>

#define LED_OFF LOW // adjust for your board
#define LED_ON HIGH // adjust for your board

#ifdef __AVR__
#pragma message "INFO: Compiling with AVR F() macro enabled."
#endif

// SERIAL

bool isBlinking = false;     // Indicates whether blinking is active or not
uint8_t ledStatus = LED_OFF; // BUILTIN_LED status (OFF/ON)
uint8_t blinkingCnt = 0;     // Number of led status changes before turning off blinking
uint32_t blinkingTime = 0;   // Time of led status change
uint32_t blinkingLast = 0;   // Last millis() in which the status of the led was changed

SerialCmd mySerCmd(Serial); // Initialize the SerialCmd constructor using the "Serial" port

// Omni4WD Omni(&wheel1,&wheel2,&wheel3,&wheel4);

/// SERIAL Functions

// ------------------- User functions --------------------

void sendOK(void)
{
#ifdef __AVR__
  mySerCmd.Print(F("OK \r\n"));
#else
  mySerCmd.Print((char *)"OK \r\n");
#endif
}

// --------------- Functions for SerialCmd ---------------

void set_LEDON(void)
{
  isBlinking = false;
  ledStatus = LED_ON;
  digitalWrite(LED_BUILTIN, LED_ON);
  digitalWrite(13, LED_ON);
  getOmni().setMotorAllAdvance(5000);
  sendOK();
}

void set_LEDOF(void)
{
  isBlinking = false;
  ledStatus = LED_OFF;
  digitalWrite(LED_BUILTIN, LED_OFF);
  digitalWrite(13, LED_OFF);
  getOmni().setMotorAllStop();
  sendOK();
}

void set_LEDBL(void)
{
  char *sParam;
  //
  sParam = mySerCmd.ReadNext();
  if (sParam == NULL)
  {
#ifdef __AVR__
    mySerCmd.Print(F("ERROR: Missing blinking time \r\n"));
#else
    mySerCmd.Print((char *)"ERROR: Missing blinking time \r\n");
#endif
    return;
  }
  blinkingCnt = 0;
  blinkingTime = strtoul(sParam, NULL, 10);
  blinkingLast = millis();
  isBlinking = true;
  sendOK();
}

#ifdef __AVR__
void set_NLBL(void)
{
  mySerCmd.AddCmd(F("LEDBL"), SERIALCMD_FROMALL, NULL);
  sendOK();
}

void set_YLBL(void)
{
  mySerCmd.AddCmd(F("LEDBL"), SERIALCMD_FROMALL, set_LEDBL);
  sendOK();
}
#else
void set_NLBL(void)
{
  mySerCmd.AddCmd("LEDBL", SERIALCMD_FROMALL, NULL);
  sendOK();
}

void set_YLBL(void)
{
  mySerCmd.AddCmd("LEDBL", SERIALCMD_FROMALL, set_LEDBL);
  sendOK();
}
#endif

// WHEELS

/*

            \                    /
   wheel1   \                    /   wheel4
   Left     \                    /   Right


                              power switch

            /                    \
   wheel2   /                    \   wheel3
   Right    /                    \   Left

 */

irqISR(irq1, isr1);
MotorWheel wheel1(3, 2, 4, 5, &irq1);

irqISR(irq2, isr2);
MotorWheel wheel2(11, 12, 14, 15, &irq2);

irqISR(irq3, isr3);
MotorWheel wheel3(9, 8, 16, 17, &irq3);

irqISR(irq4, isr4);
MotorWheel wheel4(10, 7, 18, 19, &irq4);

Omni4WD getOmni()
{
  return Omni4WD::Omni4WD(&wheel1, &wheel2, &wheel3, &wheel4);
}

void setupWheels()
{
  // TCCR0B=TCCR0B&0xf8|0x01;    // warning!! it will change millis()
  TCCR1B = TCCR1B & 0xf8 | 0x01; // Pin9,Pin10 PWM 31250Hz
  TCCR2B = TCCR2B & 0xf8 | 0x01; // Pin3,Pin11 PWM 31250Hz
  getOmni().PIDEnable(0.31, 0.01, 0, 10);

  // TODO: make some error handling to make
  // sure that we have initialized all 4 wheels.
  // if they are not all connected it will cause the entire program to fail
}

void setupSerial()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(LED_BUILTIN, ledStatus);
  digitalWrite(13, ledStatus);
  Serial.begin(9600);
  while (!Serial)
    delay(500);
    //
#ifdef ARDUINO_ARCH_STM32
  for (uint8_t i = 0; i < 7; i++)
  {
    // create a 3500 msec delay with blink for STM Nucleo boards
    delay(500);
    ledStatus = !ledStatus;
    digitalWrite(LED_BUILTIN, ledStatus);
    digitalWrite(13, ledStatus);
  }
#else
  while (!Serial)
  {
    delay(100);
    ledStatus = !ledStatus;
    digitalWrite(LED_BUILTIN, ledStatus);
    digitalWrite(13, ledStatus);
  }
#endif
  //
#ifdef __AVR__
  mySerCmd.AddCmd(F("LEDON"), SERIALCMD_FROMALL, set_LEDON); // BUILTIN  LED ON
  mySerCmd.AddCmd(F("LEDOF"), SERIALCMD_FROMALL, set_LEDOF); // BUILTIN  LED OFF
  mySerCmd.AddCmd(F("LEDBL"), SERIALCMD_FROMALL, set_LEDBL); // BUILTIN  LED BLINK, period ms in parameter
  mySerCmd.AddCmd(F("SETNB"), SERIALCMD_FROMALL, set_NLBL);  // DISABLE  LEDBL command
  mySerCmd.AddCmd(F("SETYB"), SERIALCMD_FROMALL, set_YLBL);  // REENABLE LEDBL command
  //
  mySerCmd.Print(F("INFO: Program running on AVR ... \r\n"));
#else
  mySerCmd.AddCmd("LEDON", SERIALCMD_FROMALL, set_LEDON);
  mySerCmd.AddCmd("LEDOF", SERIALCMD_FROMALL, set_LEDOF);
  mySerCmd.AddCmd("LEDBL", SERIALCMD_FROMALL, set_LEDBL);
  mySerCmd.AddCmd("SETNB", SERIALCMD_FROMALL, set_NLBL);
  mySerCmd.AddCmd("SETYB", SERIALCMD_FROMALL, set_YLBL);
  //
  mySerCmd.Print((char *)"INFO: Program running ... \r\n");
#endif
}

void serialLoop()
{
  int8_t ret;
  //
  if (isBlinking && (millis() - blinkingLast > blinkingTime))
  {
    ledStatus = !ledStatus;
    digitalWrite(LED_BUILTIN, ledStatus);
    digitalWrite(13, ledStatus);
    blinkingCnt++;
    blinkingLast += blinkingTime;
  }
  //
  if (blinkingCnt >= 10)
  {
    blinkingCnt = 0;
#ifdef __AVR__
    ret = mySerCmd.ReadString(F("LEDOF"));
#else
    ret = mySerCmd.ReadString((char *)"LEDOF");
#endif
    if (ret == false)
    {
      mySerCmd.Print((char *)"INFO: Failed to process cmd ... \r\n");
      // error processing command from string ...
      // ... insert here error handling.
    }
  }
  //
  ret = mySerCmd.ReadSer();
  if (ret == 0)
  {
    mySerCmd.Print((char *)"ERROR: Urecognized command. \r\n");
#if (SERIALCMD_PUBBUFFER == 1)
    mySerCmd.Print((char *)"       line entered : ");
    mySerCmd.Print(mySerCmd.lastLine);
    mySerCmd.Print((char *)"\r\n");
#endif
  }
}

void setup()
{
  setupWheels();
  setupSerial();
}

void loop()
{
  serialLoop();

  // getOmni().demoActions(200,5000,500,false);
}