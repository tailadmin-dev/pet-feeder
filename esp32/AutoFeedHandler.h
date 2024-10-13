#ifndef AUTO_FEED_HANDLER_H
#define AUTO_FEED_HANDLER_H

extern int last_feeding;
extern int feeding_counter;
extern const int feeding_period ;
extern const int GTMOffset;
extern const int default_periodical_feeding_amount;

void setupAutoFeed();
void checkFeeding();

#endif
