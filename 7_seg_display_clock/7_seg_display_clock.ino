// script to use an 7 seg display combined with an DS1307 rtc by Krypt0n      

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <RTClib.h>
#include "Adafruit_LEDBackpack.h"


// define if you wanna have 24 hours or 12
#define TIME_24_HOUR      false

#define DISPLAY_ADDRESS   0x70


Adafruit_7segment clockDisplay = Adafruit_7segment();
RTC_DS1307 rtc = RTC_DS1307();


int hours = 0;
int minutes = 0;
int seconds = 0;


bool blinkColon = false;


void setup() {


  Serial.begin(115200);
  Serial.println("Clock starting!");

  clockDisplay.begin(DISPLAY_ADDRESS);

  rtc.begin();

  bool setClockTime = !rtc.isrunning();

  if (setClockTime) {
    Serial.println("Setting DS1307 time!");
    
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
  }
}

void loop() {

  if (minutes == 0) {

    DateTime now = rtc.now();
  
    Serial.print("Read date & time from DS1307: ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    hours = now.hour();
    minutes = now.minute();
  }

  int displayValue = hours*100 + minutes;

  if (!TIME_24_HOUR) {

    if (hours > 12) {
      displayValue -= 1200;
    }

    else if (hours == 0) {
      displayValue += 1200;
    }
  }


  clockDisplay.print(displayValue, DEC);



  if (TIME_24_HOUR && hours == 0) {

    clockDisplay.writeDigitNum(1, 0);

    if (minutes < 10) {
      clockDisplay.writeDigitNum(3, 0);
    }
  }



  blinkColon = !blinkColon;
  clockDisplay.drawColon(blinkColon);

  clockDisplay.writeDisplay();


  delay(1000);


  seconds += 1;

  if (seconds > 59) {
    seconds = 0;
    minutes += 1;


    if (minutes > 59) {
      minutes = 0;
      hours += 1;

      if (hours > 23) {
        hours = 0;
      }
    }
  }


}
