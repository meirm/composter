#include "Arduino.h"
#include "composter.h"

COMPOSTER::COMPOSTER(int IO_Motor, int IO_Dir, int IO_LoadInterrupt, int IO_DischargeInterrupt) {
  relay_motor = IO_Motor;
  loadIntIO = IO_LoadInterrupt;
  disIntIO = IO_DischargeInterrupt;
  direction = false;
  relay_direction = IO_Dir;
  pinMode(relay_motor, OUTPUT);
  pinMode(relay_direction, OUTPUT);
  pinMode(IO_LoadInterrupt, INPUT_PULLUP);
  pinMode(IO_DischargeInterrupt, INPUT_PULLUP);
  this->stop();
  this->lastSwitch = millis();
  this->timeOn = 2 * 60 * 1000;
  this->timeOff = 2 * 60 * 1000;
  this->maxcycles = 4;
}

void COMPOSTER::setMaxCycles(int max){
  this->maxcycles = max;
}

void COMPOSTER::stopCycle(){
  this->stop();
  this->currCycle = this->maxcycles;
}

void COMPOSTER::runCycle(int numberCycles = 999) {
  this->maxcycles = ((numberCycles > 1) && (numberCycles < 999) ? numberCycles : 999);
  this->currCycle = 1;
}

unsigned long COMPOSTER::secsLastSwitch(){
  return (millis() - this->lastSwitch) / 1000; 
}

void COMPOSTER::setTimeOff(int t){
  this->timeOff = t * 60000;
}

void COMPOSTER::setTimeOn(int t){
  this->timeOn = t * 60000;
}

bool COMPOSTER::isCycling() {
  if (this->maxcycles > this->currCycle) {
    return true;
  } else {
    return false;
  }
}

int COMPOSTER::getTimeOn(){
  return this->timeOn / 1000;
}
int COMPOSTER::getTimeOff(){
  return this->timeOff / 1000;
}

int COMPOSTER::getMaxCycles(){
  return this->maxcycles;
}

int COMPOSTER::getRemindingCycles(){
  return this->maxcycles - this->currCycle;
}

bool COMPOSTER::isRunning() {
  return this->turning;
}

void COMPOSTER::restart(){
  digitalWrite(relay_motor, LOW);  //NO
  digitalWrite(relay_direction,this->direction`);
  digitalWrite(relay_motor, HIGH);  //NO
}
void COMPOSTER::run() {
  turning = true;
  digitalWrite(relay_motor, HIGH);  //NO
}

void COMPOSTER::switchDir(){
  this->direction = ! this->direction;
  if(this->isRunning()) {this->restart();}
}

void COMPOSTER::stop() {
  turning = false;
  this->_stopOnLoad = false;
  this->_stopOnDischarge = false;
  digitalWrite(relay_motor, LOW);  //NO
}

bool COMPOSTER::stopOnLoad() {
  return this->_stopOnLoad;
}
bool COMPOSTER::stopOnDischarge() {
  return this->_stopOnDischarge;
}


void COMPOSTER::stopOnLoad(bool veritas) {
  this->_stopOnLoad = veritas;
}
void COMPOSTER::stopOnDischarge(bool veritas) {
  this->_stopOnDischarge = veritas;
}

void COMPOSTER::beat() {
  if (this->isCycling()) {
    if ((this->turning) && (millis() - this->lastSwitch > this->timeOn)) {
      this->stop();
      this->lastSwitch = millis();
    }
    if ((!this->turning) && (millis() - this->lastSwitch > this->timeOff)) {
      if (this->currCycle < this->maxcycles){
        this->currCycle++;
        this->run();
      }
      this->lastSwitch = millis();
    }
  }
  if ((this->_stopOnLoad) && (this->turning) && (digitalRead(loadIntIO) == LOW)) {
    this->stop();
    this->_stopOnLoad = false;
  }
  if ((this->_stopOnDischarge) && (this->turning) && (digitalRead(disIntIO) == LOW)) {
    this->stop();
    this->_stopOnDischarge = false;
  }
}