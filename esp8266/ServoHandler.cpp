#include <Arduino.h>
#include "ServoHandler.h"
#include "MqttHandler.h"
#include "config.h"

Servo servo;

void feed(int amount) {
  Serial.println("Besleme başlatılıyor...");

  servo.attach(servoPin, 1000, 2000);
  servo.writeMicroseconds(1500);

  for (int i = 0; i < amount; i++) {
    int currentAngle = 0;

    for (int j = 0; j < 10; j++) {
      // Servo motoru ileri geri titreşim hareketi ile mama sıkışmasını önleme
      servo.write(1800);
      delay(30);
      servo.writeMicroseconds(1500);
      delay(50);
      servo.writeMicroseconds(1200);
      delay(10);
    }

    // Motoru durdur
    servo.writeMicroseconds(1500);
    delay(1000);
  }

  sendFeededMessage(amount);

  servo.detach();
}
