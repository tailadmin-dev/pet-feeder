#include "WifiHandler.h"
#include "LedHandler.h"
#include "config.h"

void startWiFi() {
  WiFi.mode(WIFI_STA);         // WiFi'yi sadece istemci modunda çalışacak şekilde ayarla
  WiFi.disconnect();           // Wifi'yi önce sıfırla
  WiFi.begin(ssid, password);  // WiFi'ye bağlanmaya başla

  Serial.print("WiFi bağlantısı kuruluyor");

  int retry_count = 0;
  while (WiFi.status() != WL_CONNECTED) {  // Bağlantı kurulana kadar bekle
    delay(1000);
    Serial.print(".");

    currentLedColor = "red";

    retry_count++;
    if (retry_count > 20) {  // Bağlantı denemesi 20'yi geçerse bağlantıyı yeniden başlat
      Serial.println("WiFi bağlantısı kurulamadı, yeniden başlatılıyor...");
      resetWiFi();
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi bağlantısı başarılı!");
    Serial.print("IP Adresi: ");
    Serial.println(WiFi.localIP());
    currentLedColor = "green";
  }
}

void resetWiFi() {
  WiFi.disconnect();           // Mevcut bağlantıyı kes
  delay(1000);                 // Kısa bir süre bekle
  WiFi.begin(ssid, password);  // Bağlantıyı yeniden başlat
  startWiFi();                 // Bağlantı sürecini yeniden başlat
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi bağlantısı kesildi, yeniden bağlanmaya çalışılıyor...");
    resetWiFi();  // Bağlantı kesildiyse yeniden başlat
  }
}
