/*
  REMEMBER TO SET SERIAL MONITOR TO CARRIAGE RETURN!

*/
#include <FastLED.h>

#include <stdlib.h>
#include <SerialCmd.h>

#define LED_OFF   LOW               // adjust for your board
#define LED_ON    HIGH              // adjust for your board

#ifdef __AVR__
#pragma message "INFO: Compiling with AVR F() macro enabled."
#endif

// LED 
// #define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    10
#define FRAMES_PER_SECOND  240
#define HUE 60
CRGB leds[NUM_LEDS];
#define PIN_LED_DATA 13
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

bool     isBlinking   = false;      // Indicates whether blinking is active or not
uint8_t  ledStatus    = LED_OFF;    // BUILTIN_LED status (OFF/ON)
uint8_t  blinkingCnt  = 0;          // Number of led status changes before turning off blinking
uint32_t blinkingTime = 0;          // Time of led status change
uint32_t blinkingLast = 0;          // Last millis() in which the status of the led was changed

SerialCmd mySerCmd ( Serial );      // Initialize the SerialCmd constructor using the "Serial" port

// ------------------- User functions --------------------

void sendOK ( void ) {
#ifdef __AVR__
   mySerCmd.Print ( F ( "OK \r\n" ) );
#else
   mySerCmd.Print ( ( char * ) "OK \r\n" );
#endif
}

// --------------- Functions for SerialCmd ---------------

void set_LEDON ( void ) {
   isBlinking = false;
   ledStatus  = LED_ON;
   digitalWrite ( LED_BUILTIN, LED_ON );
  //  drawLine(75, leds);
   sendOK();
}

void set_LEDOF ( void ) {
   isBlinking = false;
   ledStatus  = LED_OFF;
  //  fadeToBlackBy( leds, NUM_LEDS, 10);
   digitalWrite ( LED_BUILTIN, LED_OFF );
   sendOK();
}

void set_LEDBL ( void ) {
   char *   sParam;
   //
   sParam = mySerCmd.ReadNext();
   if ( sParam == NULL ) {
#ifdef __AVR__
      mySerCmd.Print ( F ( "ERROR: Missing blinking time \r\n" ) );
#else
      mySerCmd.Print ( ( char * ) "ERROR: Missing blinking time \r\n" );
#endif
      return;
   }
   blinkingCnt  = 0;
   blinkingTime = strtoul ( sParam, NULL, 10 );
   blinkingLast = millis();
   isBlinking = true;
   sendOK();
}

#ifdef __AVR__
void set_NLBL ( void ) {
   mySerCmd.AddCmd ( F ( "LEDBL" ) , SERIALCMD_FROMALL, NULL );
   sendOK();
}

void set_YLBL ( void ) {
   mySerCmd.AddCmd ( F ( "LEDBL" ) , SERIALCMD_FROMALL, set_LEDBL );
   sendOK();
}
#else
void set_NLBL ( void ) {
   mySerCmd.AddCmd ( "LEDBL", SERIALCMD_FROMALL, NULL );
   sendOK();
}

void set_YLBL ( void ) {
   mySerCmd.AddCmd ( "LEDBL", SERIALCMD_FROMALL, set_LEDBL );
   sendOK();
}
#endif

// ----------------------- setup() -----------------------

void setup() {
   delay ( 500 );
   pinMode ( LED_BUILTIN, OUTPUT );
   digitalWrite ( LED_BUILTIN, ledStatus );
   Serial.begin ( 9600 );
   while ( !Serial ) delay ( 500 );
   //
#ifdef ARDUINO_ARCH_STM32
   for ( uint8_t i = 0; i < 7; i++ ) {
      // create a 3500 msec delay with blink for STM Nucleo boards
      delay ( 500 );
      ledStatus = !ledStatus;
      digitalWrite ( LED_BUILTIN, ledStatus );
   }
#else
   while ( !Serial ) {
      delay ( 100 );
      ledStatus = !ledStatus;
      digitalWrite ( LED_BUILTIN, ledStatus );
   }
#endif
   //
#ifdef __AVR__
   mySerCmd.AddCmd ( F ( "LEDON" ) , SERIALCMD_FROMALL, set_LEDON ); // BUILTIN  LED ON
   mySerCmd.AddCmd ( F ( "LEDOF" ) , SERIALCMD_FROMALL, set_LEDOF ); // BUILTIN  LED OFF
   mySerCmd.AddCmd ( F ( "LEDBL" ) , SERIALCMD_FROMALL, set_LEDBL ); // BUILTIN  LED BLINK, period ms in parameter
   mySerCmd.AddCmd ( F ( "SETNB" ) , SERIALCMD_FROMALL, set_NLBL );  // DISABLE  LEDBL command
   mySerCmd.AddCmd ( F ( "SETYB" ) , SERIALCMD_FROMALL, set_YLBL );  // REENABLE LEDBL command
   //
   mySerCmd.Print ( F ( "INFO: Program running on AVR ... \r\n" ) );
#else
   mySerCmd.AddCmd ( "LEDON", SERIALCMD_FROMALL, set_LEDON );
   mySerCmd.AddCmd ( "LEDOF", SERIALCMD_FROMALL, set_LEDOF );
   mySerCmd.AddCmd ( "LEDBL", SERIALCMD_FROMALL, set_LEDBL );
   mySerCmd.AddCmd ( "SETNB", SERIALCMD_FROMALL, set_NLBL );
   mySerCmd.AddCmd ( "SETYB", SERIALCMD_FROMALL, set_YLBL );
   //
   mySerCmd.Print ( ( char * ) "INFO: Program running ... \r\n" );
#endif

   // init fastled
  // FastLED.addLeds<LED_TYPE, PIN_LED_DATA, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}

// ----------------------- loop() ------------------------

void loop() {
   int8_t ret;
   //
   if ( isBlinking && ( millis() - blinkingLast > blinkingTime ) ) {
      ledStatus = !ledStatus;
      digitalWrite ( LED_BUILTIN, ledStatus );
      blinkingCnt++;
      blinkingLast += blinkingTime;
   }
   //
   if ( blinkingCnt >= 10 ) {
      blinkingCnt  = 0;
#ifdef __AVR__
      ret = mySerCmd.ReadString ( F ( "LEDOF" ) );
#else
      ret = mySerCmd.ReadString ( ( char * ) "LEDOF" );
#endif
      if ( ret == false ) {
        mySerCmd.Print ( ( char * ) "INFO: Failed to process cmd ... \r\n" );
         // error processing command from string ...
         // ... insert here error handling.

      }
   }
   //
   ret = mySerCmd.ReadSer();
   if ( ret == 0 ) {
      mySerCmd.Print ( ( char * ) "ERROR: Urecognized command. \r\n" );
#if ( SERIALCMD_PUBBUFFER == 1 )
      mySerCmd.Print ( ( char * ) "       line entered : " );
      mySerCmd.Print ( mySerCmd.lastLine );
      mySerCmd.Print ( ( char * ) "\r\n" );
#endif
   }

   FastLED.delay(1000/FRAMES_PER_SECOND);
}


void drawLine(int fadeDelay, CRGB* ledArray) {
  for(int i=0; i<NUM_LEDS; i++) { // For each pixel...
    ledArray[i] = CHSV( HUE, 0, 255);
    FastLED.show();
    FastLED.delay(fadeDelay);
  }
}