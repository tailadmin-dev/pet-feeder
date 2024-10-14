#include <WiFiUdp.h>
#include <AutoFeedHandler.h>
#include <MqttHandler.h>
#include <NTPClient.h>
#include "ServoHandler.h"
#include <EEPROM.h>
#include "config.h"

// Feeding
int last_feeding = 0;
int feeding_counter = 0;

// Besleme planı
FeedingTime feedingSchedule[50];
int scheduleCount = 0;   


// NTP Client Settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", GTMOffset * 60 * 60, 36000);

void setupAutoFeed(){
  loadScheduleFromEEPROM();
  // NTP Client Start
  timeClient.begin();
}

void checkFeeding() {
  timeClient.update();

  int currentDay = timeClient.getDay();      // 0-6 (Pazar-Cumartesi)
  int currentHour = timeClient.getHours();   // 0-23
  int currentMinute = timeClient.getMinutes(); // 0-59

  
  for (int i = 0; i < scheduleCount; i++) {
    if (feedingSchedule[i].d == currentDay &&
        feedingSchedule[i].h == currentHour &&
        feedingSchedule[i].m == currentMinute) {
      
      // Aynı dakika içinde birden fazla kez tetiklenmesini önlemek için
      static unsigned long lastFeedMillis = 0;
      if (millis() - lastFeedMillis > 60000) {  // 1 dakika içinde tekrar tetiklenmesin
        feed(feedingSchedule[i].a);
        lastFeedMillis = millis();
      }
    }
  }
}

void saveScheduleToEEPROM(String sortedJson) {
  DynamicJsonDocument doc(8192);
  DeserializationError error = deserializeJson(doc, sortedJson);

  if (error) {
    Serial.print("JSON hata: ");
    Serial.println(error.c_str());
    return;
  }

  // Besleme takvimini EEPROM'a kaydet
  int scheduleSize = doc.size();
  int addr = 0;

  EEPROM.put(addr, scheduleSize);
  addr += sizeof(int);

  for (int i = 0; i < scheduleSize; i++) {
    EEPROM.put(addr, feedingSchedule[i]);  // Nesneleri sırayla EEPROM'a yaz
    addr += sizeof(FeedingTime);
  }

  EEPROM.commit();
  Serial.println("Sıralanmış plan EEPROM'a kaydedildi.");
}

void loadScheduleFromEEPROM() {
  int addr = 0;
  EEPROM.get(addr, scheduleCount);
  addr += sizeof(int);

  for (int i = 0; i < scheduleCount; i++) {
    EEPROM.get(addr, feedingSchedule[i]);
    addr += sizeof(FeedingTime);
  }
}
