#include "AutoFeedHandler.h"
#include "MqttHandler.h"
#include "ServoHandler.h"
#include "LedHandler.h"
#include "config.h"
#include <Arduino.h>
#include <FS.h>

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastPingTime = 0;
unsigned long lastFeedMessageTime = 0;              // Son feed mesajının zamanı
const unsigned long messageValidityPeriod = 60000;  // 1 dakika (milisaniye cinsinden)
bool initialConnection = true;                      // Cihazın ilk açılış durumu

void setupMQTT() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setBufferSize(2048);
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

    if (client.connect("ESP8266Client")) {
      Serial.println("Bağlandı!");

      //Subscribe
      if (client.subscribe(feed_channel)) {
          Serial.println("MQTT Subscribe başarılı: feed_channel");
      } else {
          Serial.println("MQTT Subscribe HATASI: feed_channel");
      }

      if (client.subscribe(ping_channel)) {
          Serial.println("MQTT Subscribe başarılı: ping_channel");
      } else {
          Serial.println("MQTT Subscribe HATASI: ping_channel");
      }

      if (client.subscribe(feedback_channel, 1)) {
          Serial.println("MQTT Subscribe başarılı: feedback_channel");
      } else {
          Serial.println("MQTT Subscribe HATASI: feedback_channel");
      }

      if (client.subscribe(request_schedule_channel, 1)) {
          Serial.println("MQTT Subscribe başarılı: request_schedule_channel");
      } else {
          Serial.println("MQTT Subscribe HATASI: request_schedule_channel");
      }

      if (client.subscribe(update_schedule_channel, 1)) {
          Serial.println("MQTT Subscribe başarılı: update_schedule_channel");
      } else {
          Serial.println("MQTT Subscribe HATASI: update_schedule_channel");
      }

      //Publish
      client.publish(feedback_channel, "MQTT bağlantısı kuruldu");
      currentLedColor = "green";
    } else {
      retry_count++;
      Serial.print("Bağlantı başarısız, durum: ");
      Serial.print(client.state());

      if (retry_count > 10) {  // Bağlantı denemesi 20'yi geçerse bağlantıyı yeniden başlat
        Serial.println("Bağlantı denemeleri başarısız oldu, WiFi yeniden başlatılıyor...");
        WiFi.disconnect();
        WiFi.begin(ssid, password);
        delay(5000);
      }
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.println("MQTT Callback Tetiklendi!"); 

  char msg[2048];
  if (length < 2048) {
    strncpy(msg, (char*)message, length);
    msg[length] = '\0';  // Null terminator ekleyin
    Serial.print("Alınan mesaj: ");
    Serial.println(msg);
  } else {
    Serial.println("Mesaj çok uzun!");
  }

  Serial.print("Gelen Topic: ");
  Serial.println(topic);

  if (strcmp(topic, feed_channel) == 0) {
    Serial.println("Feed kanalı mesajı alındı!");
    unsigned long currentTime = millis();

    // Eğer cihaz ilk açılışta değilse ve mesaj geçerli sürede gelmişse işleme al
    if (!initialConnection && (currentTime - lastFeedMessageTime <= messageValidityPeriod)) {

      int amount = 0;

      // JSON mesajını parse et
      StaticJsonDocument<2048> doc;
      DeserializationError error = deserializeJson(doc, msg);

      // Hata kontrolü
      if (error) {
        Serial.print("JSON parse hatası: ");
        Serial.println(error.c_str());
        return;
      }

      // JSON verilerini oku
      amount = doc["amount"];

      if (amount <= 0) {  // ✅ Negatif veya geçersiz değerleri engelle
          Serial.println("MQTT Hata: Geçersiz besleme miktarı!");
          return;
      }

      // Servo motoru döndürme fonksiyonunu çağır
      feed(amount);
    }else{
      Serial.println("MQTT Hata: Geçersiz besleme mesajı.");
    }

    lastFeedMessageTime = currentTime;  // Mesaj zamanını güncelle
    initialConnection = false;          // İlk bağlantı durumu sıfırlanır
  }
  else if (String(topic) == request_schedule_channel) {
    Serial.println("Besleme takvimi görüntüleme talebi için mesaj alındı.");
    // Mevcut besleme planı talebi
    String schedule = loadScheduleFromSPIFFS();
    client.publish(response_schedule_channel, schedule.c_str());
  }
  else if (String(topic) == update_schedule_channel) {
    Serial.println("Besleme takvimi güncellenmesi talebi alındı.");
    // Besleme planı güncellemesi
    Serial.print(String(msg));
    updateScheduleFromMQTT(msg);
    client.publish(feedback_channel, "Besleme planı güncellendi");
  }

  Serial.print(String(msg));
}

void checkMQTTConnection() {
  if (!client.connected()) {
    Serial.println("MQTT bağlantısı koptu, yeniden bağlanılıyor...");
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
  Serial.println("MQTT Güncelleme Mesajı Alındı.");
  Serial.print(String(jsonMessage));

  // Gelen JSON'u sıralı hale getir
  String sortedJson = sortFeedingSchedule(jsonMessage);

  // Sıralanmış planı belleğe kaydet
  saveScheduleToSPIFFS(sortedJson);

   // Geri bildirim gönder
  client.publish(feedback_channel, "Besleme planı sıralandı ve kaydedildi.");
}

String getScheduleAsJSON() {
  // Bellekten veriyi yükle
  return loadScheduleFromSPIFFS();
}

String sortFeedingSchedule(String unsortedJson) {
  StaticJsonDocument<2048> doc;
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
