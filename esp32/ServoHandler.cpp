#include <Arduino.h>
#include "ServoHandler.h"
#include "MqttHandler.h"
#include "config.h"

Servo servo;

void feed(int amount) {
  servo.setPeriodHertz(50);
  servo.attach(servoPin, 1000, 2000);  // Servo motoru bağla
  servo.writeMicroseconds(1500);


  for (int i = 0; i < amount; i++) {
    // Tablayı döndürme
    servo.writeMicroseconds(1000);  // Motoru ileri döndür
    delay(350);                     // Döndürme işlemi süresi

    // Motoru durdur
    servo.writeMicroseconds(1500);
    delay(1000);
  }

  sendFeededMessage(amount);

  servo.detach();
}
