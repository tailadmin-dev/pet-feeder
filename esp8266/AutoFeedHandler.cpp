#include <WiFiUdp.h>
#include <NTPClient.h>
#include "AutoFeedHandler.h"
#include "MqttHandler.h"
#include "ServoHandler.h"
#include "config.h"
#include <FS.h>

// Feeding
int last_feeding_day = -1;
int last_feeding_hour = -1;
int last_feeding_minute = -1;
int feeding_counter = 0;

// Besleme planı
FeedingTime feedingSchedule[50];
int scheduleCount = 0;   


// NTP Client Settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", GTMOffset * 3600, 60000);

void setupAutoFeed(){
   if (!SPIFFS.begin()) {
    Serial.println("SPIFFS başlatılamadı!");
    return;
  }

  loadScheduleFromSPIFFS();
  // NTP Client Start
  timeClient.begin();
  Serial.println("Otomatik besleme sistemi başlatıldı.");
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
      if (last_feeding_day == currentDay &&
          last_feeding_hour == currentHour &&
          last_feeding_minute == currentMinute) {
        return;
      }

      Serial.println("Zaman eşleşti, besleme yapılıyor...");
      feed(feedingSchedule[i].a);

      // Son besleme zamanı güncelleniyor
      last_feeding_day = currentDay;
      last_feeding_hour = currentHour;
      last_feeding_minute = currentMinute;
    }
  }
}

void saveScheduleToSPIFFS(String sortedJson) {
  File file = SPIFFS.open("/feedingSchedule.json", "w");
  if (!file) {
    Serial.println("SPIFFS: Dosya açılamadı!");
    return;
  }

  file.print(sortedJson);
  file.close();
  Serial.println("Besleme programı SPIFFS'e kaydedildi.");
}

String loadScheduleFromSPIFFS() {
  File file = SPIFFS.open("/feedingSchedule.json", "r");
  if (!file) {
    Serial.println("SPIFFS: Kayıtlı program bulunamadı!");
    return "{}";
  }

  String jsonContent;
  while (file.available()) {
    jsonContent += (char)file.read();
  }
  file.close();

  StaticJsonDocument<2048> doc;
  DeserializationError error = deserializeJson(doc, jsonContent);
  if (error) {
    Serial.println("JSON hata: " + String(error.c_str()));
    return "{}";
  }

  scheduleCount = doc.size();
  if (scheduleCount > 50) scheduleCount = 50;

  for (int i = 0; i < scheduleCount; i++) {
    feedingSchedule[i].d = doc[i]["d"];
    feedingSchedule[i].h = doc[i]["h"];
    feedingSchedule[i].m = doc[i]["m"];
    feedingSchedule[i].a = doc[i]["a"];
  }

  Serial.println("Besleme programı SPIFFS'ten yüklendi.");

  return jsonContent;
}
