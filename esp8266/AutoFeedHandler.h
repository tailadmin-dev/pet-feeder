#ifndef AUTO_FEED_HANDLER_H
#define AUTO_FEED_HANDLER_H

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FS.h>

struct FeedingTime {
  int d;  // 0: Pazar, 1: Pazartesi, ..., 6: Cumartesi
  int h;
  int m;
  int a;
};

extern int last_feeding_day;
extern int last_feeding_hour;
extern int last_feeding_minute;
extern int feeding_counter;
extern const int feeding_period ;
extern const int GTMOffset;
extern const int default_periodical_feeding_amount;
extern FeedingTime feedingSchedule[50];  // Maksimum 50 besleme zamanı
extern int scheduleCount;

void setupAutoFeed();
void checkFeeding();
void saveScheduleToSPIFFS(String sortedJson);
String loadScheduleFromSPIFFS();

#endif
