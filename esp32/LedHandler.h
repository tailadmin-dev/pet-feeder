#ifndef LED_HANDLER_H
#define LED_HANDLER_H

// IR Var yok sensörü ayarları
extern const int ledRedPin;
extern const int ledGreenPin;
extern const int ledBluePin;
extern const char* currentLedColor;

void setupLeds();
void checkLedColor();
void blink(int red, int green, int blue);

#endif