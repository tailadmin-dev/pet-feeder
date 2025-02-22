#include "WifiHandler.h"
#include "LedHandler.h"
#include "config.h"

void ICACHE_FLASH_ATTR startWiFi() {
  WiFi.mode(WIFI_STA);         // WiFi'yi sadece istemci modunda çalışacak şekilde ayarla
  WiFi.disconnect();           // Wifi'yi önce sıfırla
  Serial.print("WiFi bağlantısı kuruluyor");

  WiFi.begin(ssid, password);  // WiFi'ye bağlanmaya başla

  int retry_count = 0;
  while (WiFi.status() != WL_CONNECTED && retry_count < 20) {  // Bağlantı kurulana kadar bekle
    delay(1000);
    Serial.print(".");

    currentLedColor = "red";

    retry_count++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi bağlantısı başarılı!");
    Serial.print("IP Adresi: ");
    Serial.println(WiFi.localIP());
    currentLedColor = "green";
  }else{
    Serial.println("WiFi bağlantısı kurulamadı, yeniden başlatılıyor...");
    resetWiFi();
  }
}

void resetWiFi() {
  Serial.println("WiFi bağlantısı sıfırlanıyor...");
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
