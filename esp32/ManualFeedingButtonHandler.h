#ifndef MANUAL_FEEDING_BUTTON_HANDLER_H
#define MANUAL_FEEDING_BUTTON_HANDLER_H

extern int buttonPin;           // Butonun bağlı olduğu pin
extern unsigned long checkInterval; // Kaç saniyede bir metodun çağrılacağı

void setupButton();
void checkButton();

#endif
