#include <Arduino.h>
#include "ServoHandler.h"
#include "MqttHandler.h"
#include "config.h"

Servo servo;
const int SECTIONS = 8; // Tekerleğin 8 gözü var
const int ANGLE_PER_SECTION = 360 / SECTIONS; // Her göz için açı: 45 derece

void feed(int amount) {
  Serial.println("Besleme başlatılıyor...");

  servo.attach(servoPin, 500, 2500);
  
  // Servonun başlangıç pozisyonunda olduğundan emin ol
  servo.writeMicroseconds(1500); // 1500 genellikle servo'nun orta/durma pozisyonudur
  delay(500);

  for (int i = 0; i < amount; i++) {
    Serial.print(i+1);
    Serial.println(". beslemede tekerlek 45 derece döndürülüyor");
    
    // Hareketi başlat
    servo.writeMicroseconds(1200); // Saat yönünde yavaş hareket
    
    // 45 derecelik dönüş için gereken süre (kalibre etmeniz gerekebilir)
    delay(500); // Bu değeri servo motorunuzun hızına göre ayarlayın
    
    // Motoru durdur
    servo.writeMicroseconds(1500);
    
    // Bir sonraki besleme öncesi bekle
    delay(1000);
  }

  sendFeededMessage(amount);

  servo.detach();
}
