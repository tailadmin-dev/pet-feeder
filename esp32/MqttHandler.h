#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H
#define MQTT_MAX_PACKET_SIZE 4096

#include <WiFi.h>
#include <PubSubClient.h> // MQTT kütüphanesi
#include <ArduinoJson.h>

// MQTT sunucusu ayarları
extern const char* mqtt_server;
extern const int mqtt_port;

// MQTT client objesi
extern WiFiClient espClient;
extern PubSubClient client;

extern const char* feed_channel;
extern const char* ping_channel;
extern const char* feedback_channel;

void setupMQTT();
void reconnectMQTT();
void callback(char* topic, byte* message, unsigned int length);
void checkMQTTConnection();
void sendPingMessage();
void checkPingTime();
void sendFeededMessage(int amount);
void updateScheduleFromMQTT(String jsonMessage);
String sortFeedingSchedule(String jsonMessage);
String getScheduleAsJSON();

#endif
