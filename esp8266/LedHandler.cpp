#include <Arduino.h>
#include "LedHandler.h"
#include "config.h"

// Current color
const char* currentLedColor = "white";

void setupLeds() {
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
}

inline void blink(int red, int green, int blue) {
  analogWrite(ledRedPin, red);
  analogWrite(ledGreenPin, green);
  analogWrite(ledBluePin, blue);
}

void checkLedColor() {
  if (String(currentLedColor) == "red") {
    blink(255, 0, 0);
  } else if (String(currentLedColor) == "green") {
    blink(0, 255, 0);
  } else if (String(currentLedColor) == "blue") {
    blink(0, 0, 255);
  }else if (String(currentLedColor) == "yellow") {
    blink(255, 255, 0);
  }else if (String(currentLedColor) == "white") {
    blink(255, 255, 255);
  }else{
    blink(0, 0, 0);
  }}
