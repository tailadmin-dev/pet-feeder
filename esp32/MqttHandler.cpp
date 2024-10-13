#include "MqttHandler.h"
#include "ServoHandler.h"
#include "LedHandler.h"
#include "config.h"

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPingTime = 0;


unsigned long lastFeedMessageTime = 0;              // Son feed mesajının zamanı
const unsigned long messageValidityPeriod = 60000;  // 1 dakika (milisaniye cinsinden)
bool initialConnection = true;                      // Cihazın ilk açılış durumu

void setupMQTT() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
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
      Serial.println("Bağlandı!");
      client.subscribe(feed_channel);
      client.subscribe(ping_channel);
      client.subscribe(feedback_channel);
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
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }

  if (String(topic) == feed_channel) {
    unsigned long currentTime = millis();

    // Eğer cihaz ilk açılışta değilse ve mesaj geçerli sürede gelmişse işleme al
    if (!initialConnection && (currentTime - lastFeedMessageTime <= messageValidityPeriod)) {

      int amount = 0;

      // JSON mesajını parse et
      DynamicJsonDocument doc(1024);
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
    }

    lastFeedMessageTime = currentTime;  // Mesaj zamanını güncelle
    initialConnection = false;          // İlk bağlantı durumu sıfırlanır
  }
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
