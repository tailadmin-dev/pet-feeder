#ifndef SERVO_HANDLER_H
#define SERVO_HANDLER_H

#include <Servo.h>

extern int servoPin;             // Servo motorun bağlı olduğu pin
extern int numberOfSections;     // Daire üzerindeki bölme sayısı

void feed(int amount);        // Motoru belirtilen miktar kadar döndürme metodu

#endif
