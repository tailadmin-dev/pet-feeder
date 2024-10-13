#include <WiFiUdp.h>
#include <NTPClient.h>
#include "ServoHandler.h"
#include "config.h"

// Feeding
int last_feeding = 0;
int feeding_counter = 0;


// NTP Client Settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", GTMOffset * 60 * 60, 36000);

void setupAutoFeed(){
  // NTP Client Start
  timeClient.begin();
}

void checkFeeding() {
  unsigned long currentMillis = millis();
  int period = feeding_period * 60 * 60 * 1000;
  if (currentMillis - last_feeding >= period) {
    Serial.println(currentMillis);
    Serial.println(last_feeding);
    Serial.println(period);
    last_feeding = currentMillis;
    feed(default_periodical_feeding_amount);
  }
}
