#include "sys/_types.h"
#ifndef _DATECALC
#define _DATECALC
#include <Arduino.h>
struct datecalc_struct {
  bool diff;
  int days;
  int hours;
  int minutes;
  int seconds;
  String time;
};

class DateCalc {
public:
  DateCalc(unsigned long timestamp);
  datecalc_struct calc();
  datecalc_struct update_and_calc(unsigned long timestamp);
  void update(unsigned long timestamp);
  datecalc_struct timeDiff(unsigned long timestamp);
  unsigned long getTimestamp();
  int hours();
  int days();
  int minutes();
  int seconds();
  unsigned long lapse(int smhd);  // in secs, mins, hours, days
private:
  unsigned long _timestamp;
  const unsigned long day = 86400000;
  const unsigned long hour = 3600000;
  const unsigned long minute = 60000;
  const unsigned long second = 1000;
};
#endif