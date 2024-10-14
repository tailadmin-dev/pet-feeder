#ifndef CONFIG_H
#define CONFIG_H

// WiFiHandler
extern const char* ssid; // Wifi adı
extern const char* password; // Wifi şifresi

//MqttHandler
extern const char* mqtt_server; // Mqtt sunucusu adresi
extern const int mqtt_port; // Mqtt sunucusunun portu
extern const unsigned long pingInterval;  // Ping periyodu
extern const char* feed_channel; // Besle mesajının gönderileceği kanal
extern const char* ping_channel; // Ping mesajının gönderileceği kanal
extern const char* feedback_channel; // Geri bildirim kanalı
extern const char* request_schedule_channel;   // Plan talep kanalı
extern const char* response_schedule_channel;  // Plan yanıt kanalı
extern const char* update_schedule_channel;    // Plan güncelleme kanalı

//AutoFeedHandler
extern const int feeding_period; // Besleme sıklığı
extern const int default_periodical_feeding_amount; // Varsayılan öğün miktarı
extern const int GTMOffset; // Zaman dilimi

//ManualFeedingButtonHandler
extern int manualFeedingButton; // Manuel besleme için düğme pini
extern unsigned long checkInterval; // Besleme yapması için düğmeye basılacak süre

//ServoHandler
extern int servoPin; // Servo motor pini

//IRHandler
extern const int irPin; // IR sensör pini

//LedHandler
extern const int ledRedPin; // Kırmızı LED pini
extern const int ledGreenPin; // Yeşil LED pini
extern const int ledBluePin; // Mavi LED pini

#endif
