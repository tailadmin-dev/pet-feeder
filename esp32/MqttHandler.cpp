#include "AutoFeedHandler.h"
#include "MqttHandler.h"
#include "ServoHandler.h"
#include "LedHandler.h"
#include "config.h"
#include <Arduino.h>

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastPingTime = 0;


unsigned long lastFeedMessageTime = 0;              // Son feed mesajının zamanı
const unsigned long messageValidityPeriod = 60000;  // 1 dakika (milisaniye cinsinden)
bool initialConnection = true;                      // Cihazın ilk açılış durumu

void setupMQTT() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setBufferSize(4096);
  client.setKeepAlive(60); 

  reconnectMQTT();
}

void reconnectMQTT() {
  int retry_count = 0;

  Serial.print("MQTT sunucusuna bağlanılıyor...");

  currentLedColor = "red";

  while (!client.connected()) {
    delay(1000);
    Serial.print(".");

    if (client.connect("ESP32Client")) {
      //Subscribe
      client.subscribe(feed_channel);
      client.subscribe(ping_channel);
      client.subscribe(feedback_channel, 1);
      client.subscribe(request_schedule_channel, 1);
      client.subscribe(update_schedule_channel, 1);

      //Publish
      Serial.println("Bağlandı!");
      client.publish(feedback_channel, "MQTT bağlantısı kuruldu");
      currentLedColor = "green";
    } else {
      retry_count++;
      if (retry_count > 20) {  // Bağlantı denemesi 20'yi geçerse bağlantıyı yeniden başlat
        Serial.print("Bağlantı başarısız, durum: ");
        Serial.print(client.state());
        delay(5000);
      }
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
    char msg[4096];
  if (length < 4096) {
    strncpy(msg, (char*)message, length);
    msg[length] = '\0';  // Null terminator ekleyin
    Serial.print("Alınan mesaj: ");
    Serial.println(msg);
  } else {
    Serial.println("Mesaj çok uzun!");
  }

  if (String(topic) == feed_channel) {
    unsigned long currentTime = millis();

    // Eğer cihaz ilk açılışta değilse ve mesaj geçerli sürede gelmişse işleme al
    if (!initialConnection && (currentTime - lastFeedMessageTime <= messageValidityPeriod)) {

      int amount = 0;

      // JSON mesajını parse et
      DynamicJsonDocument doc(8192);
      DeserializationError error = deserializeJson(doc, msg);

      // Hata kontrolü
      if (error) {
        Serial.print("JSON parse hatası: ");
        Serial.println(error.c_str());
        return;
      }

      // JSON verilerini oku
      amount = doc["amount"];

      // Servo motoru döndürme fonksiyonunu çağır
      feed(amount);
    }else{
      Serial.println('ahata var');
    }

    lastFeedMessageTime = currentTime;  // Mesaj zamanını güncelle
    initialConnection = false;          // İlk bağlantı durumu sıfırlanır
  }
  else if (String(topic) == request_schedule_channel) {
    // Mevcut besleme planı talebi
    String schedule = getScheduleAsJSON();
    client.publish(response_schedule_channel, schedule.c_str());
  }
  else if (String(topic) == update_schedule_channel) {
    // Besleme planı güncellemesi
        Serial.print(String(msg));
    updateScheduleFromMQTT(msg);
    client.publish(feedback_channel, "Besleme planı güncellendi");
  }

  Serial.print(String(msg));
}

void checkMQTTConnection() {
  if (!client.connected()) {
    reconnectMQTT();
  }

  // Ping
  checkPingTime();

  client.loop();
}

void sendPingMessage() {
  String pingMsg = "Tarih ve Saat: " + String(__DATE__) + " " + String(__TIME__);
  client.publish(ping_channel, pingMsg.c_str());
}

void sendFeededMessage(int amount) {
  String response = String(amount) + " öğünlük besleme yapıldı.";
  Serial.println(response.c_str());
  client.publish(feedback_channel, response.c_str());
}

void checkPingTime() {
  unsigned long currentTime = millis();
  if (currentTime - lastPingTime >= pingInterval) {
    sendPingMessage();
    lastPingTime = currentTime;
  }
}

void updateScheduleFromMQTT(String jsonMessage) {
    Serial.print(String(jsonMessage));

  
  // Gelen JSON'u sıralı hale getir
  String sortedJson = sortFeedingSchedule(jsonMessage);

  // Sıralanmış planı EEPROM'a kaydet
  saveScheduleToEEPROM(sortedJson);

   // Geri bildirim gönder
  client.publish(feedback_channel, "Besleme planı sıralandı ve kaydedildi.");
}

String sortFeedingSchedule(String unsortedJson) {
  DynamicJsonDocument doc(8192);
  DeserializationError error = deserializeJson(doc, unsortedJson);

  if (error) {
    Serial.print("JSON hata: ");
    Serial.println(error.c_str());
    return unsortedJson; // Hata durumunda sıralanmamış veriyi geri döndürüyoruz
  }

  // FeedingTime verilerini bir vektöre yükle
  std::vector<FeedingTime> schedule;
  for (JsonObject obj : doc.as<JsonArray>()) {
    FeedingTime time;
    time.d = obj["d"];
    time.h = obj["h"];
    time.m = obj["m"];
    time.a = obj["a"];
    schedule.push_back(time);
  }

  // Sıralama işlemi
  std::sort(schedule.begin(), schedule.end(), [](const FeedingTime &a, const FeedingTime &b) {
    if (a.d != b.d) return a.d < b.d; // Önce gün değerine göre
    if (a.h != b.h) return a.h < b.h; // Aynı günse saat değerine göre
    return a.m < b.m;                 // Aynı saatse dakika değerine göre
  });

  // Sıralanmış veriyi JSON formatında oluştur
  DynamicJsonDocument sortedDoc(8192);
  for (const FeedingTime &time : schedule) {
    JsonObject obj = sortedDoc.createNestedObject();
    obj["d"] = time.d;
    obj["h"] = time.h;
    obj["m"] = time.m;
    obj["a"] = time.a;
  }

  // Sıralanmış JSON'u geri döndür
  String sortedJson;
  serializeJson(sortedDoc, sortedJson);
  return sortedJson;
}

String getScheduleAsJSON() {
  // EEPROM'dan veriyi yükle
  loadScheduleFromEEPROM();

  // JSON dökümantini oluştur
  DynamicJsonDocument doc(8192);
  
  // EEPROM'dan okunan feedingSchedule verilerini JSON'a dönüştür
  for (int i = 0; i < scheduleCount; i++) {
    JsonObject obj = doc.createNestedObject();
    obj["d"] = feedingSchedule[i].d;
    obj["h"] = feedingSchedule[i].h;
    obj["m"] = feedingSchedule[i].m;
    obj["a"] = feedingSchedule[i].a;
  }

  // JSON string'i olarak serialize et
  String output;
  serializeJson(doc, output);
  
  // JSON'u geri döndür
  return output;
}
