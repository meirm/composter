#include "sys/_types.h"
#include "datecalc.h"

DateCalc::DateCalc(unsigned long timestamp) {
  this->_timestamp = timestamp;
}

unsigned long DateCalc::getTimestamp() {
  return this->_timestamp;
}

void DateCalc::update(unsigned long timestamp) {
  this->_timestamp = timestamp;
}

datecalc_struct DateCalc::update_and_calc(unsigned long timestamp){
  this->update(timestamp);
  return this->calc();
}

datecalc_struct DateCalc::calc() {
  datecalc_struct new_time;
  new_time.days = this->days();
  new_time.hours = this->hours();
  new_time.minutes = this->minutes();
  new_time.seconds = this->seconds();
  new_time.time = String(new_time.days);
  new_time.time += " days " + String(new_time.hours) + ":" + String(new_time.minutes) + ":" + String(new_time.seconds); 
   return new_time;
}

datecalc_struct DateCalc::timeDiff(unsigned long timestamp) {
  unsigned long orig_timestamp = this->_timestamp;
  unsigned long tdiff = timestamp - this->_timestamp;
  datecalc_struct new_time;
  new_time = this->update_and_calc(tdiff);
  this->_timestamp = orig_timestamp;
  return new_time;
}
int DateCalc::hours() {
  return this->_timestamp % this->day / this->hour;
}
int DateCalc::days() {
  return this->_timestamp / this->day;
}
int DateCalc::minutes() {
  return this->_timestamp % this->hour / this->minute;
}
int DateCalc::seconds() {
  return this->_timestamp % this->minute / this->second;
}
unsigned long DateCalc::lapse(int smhd) {
  return 1;
}  // in secs, mins, hours, days