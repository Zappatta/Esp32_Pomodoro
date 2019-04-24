#include <Arduino.h>
#include "SSD1306.h"

SSD1306 display(0x3c, 5, 4);
const int focusButton = 14;        //gpio to use to trigger delay
const int restButton = 15;
const int focusMinutes = 25;
const int restMinutes = 5;
const int alertLED = 13;

char* curTarget = "SELECT";

int seconds = 0;
int minutes = 0;

void setupOled()
{
  display.init();
  display.flipScreenVertically();
  // display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.display();
}

void showTime()
{
  display.clear();
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 0, String(minutes) + ":" + String(seconds));
  display.drawString(0, 27, curTarget);
  display.display();
}

void checkButtons(){
  const int focusButtonRead = digitalRead(focusButton);
  const int restButtonRead = digitalRead(restButton);

  Serial.printf("focus read: %d - rest read: %d",focusButtonRead, restButtonRead );
  if(focusButtonRead == HIGH){
    seconds = 0;
    minutes = 0;
    curTarget = "FOCUS";
  }
  else if (restButtonRead == HIGH){
    seconds = 0;
    minutes = 0;
    curTarget = "REST";
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("running setup");

  pinMode(focusButton, INPUT_PULLDOWN);
  pinMode(restButton, INPUT_PULLDOWN);
  pinMode(alertLED, OUTPUT);

  setupOled();
  showTime();
}

void loop()
{

  seconds++;
  if(seconds == 60){
    minutes++;
    seconds = 0;
  }
  showTime();
  Serial.println("running main loop");

  checkButtons();

  if ((curTarget == "FOCUS" && minutes >= focusMinutes) || (curTarget == "REST" && minutes >= restMinutes))
  {
    digitalWrite(alertLED, HIGH);
  }else {
    digitalWrite(alertLED, 0);
  }
  
  delay(1000);
}

