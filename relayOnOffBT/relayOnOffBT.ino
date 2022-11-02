//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include "composter.h"
#include "datecalc.h"
#include "extras.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
DateCalc datecalc(0);
char buff[128];
bool gotNewLine = false;
int pos = 0;

COMPOSTER composter(4, 12, 14);

void setup() {
  Serial.begin(115200);
  SerialBT.begin("BIOTECH Composter");  //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}



void loop() {
  while (SerialBT.available()) {
    char c = SerialBT.read();
    if (c == '\r') continue;
    if (c == '\n') {
      //Serial.println(" Got new line!!!");
      buff[pos] = '\0';
      gotNewLine = true;
    } else {
      buff[pos++] = c;
      //Serial.write(c);
    }
  }

  if (gotNewLine) {
    Serial.print("Got new line:");
    Serial.println(buff);
    processLine();
  }
  composter.beat();
  delay(20);
}

void processLine() {
  char greetings[128];
  //sprintf(greetings, "GOT: %s\r\n", buff);
  SerialBT.print(greetings);
  pos = 0;
  gotNewLine = false;
  if (processCommand(buff)) {
    sprintf(greetings, "OK\r\n");
  } else {
    sprintf(greetings, "ERR\r\n");
  }
  SerialBT.print(greetings);
}

bool processCommand(char* buff) {
  char summary[128];
  bool retValue = false;
  if (strcmp2(buff, (char*)"OFF")) {
    composter.stop();  // NO
    retValue = true;
  } else if (strcmp2(buff, (char*)"ON")) {
    composter.run();  // NO
    retValue = true;
  } else if (strcmp2(buff, (char*)"LAST")) {
    int secs = composter.secsLastSwitch();
    sprintf(summary, "#Last switch: %02d:%02d:%02d \r\n", secs / 3600, (secs % 3600) / 60, secs % 60);
    SerialBT.print(summary);
    retValue = true;
  } else if (strcmp2(buff, (char*)"CYCLE")) {
    composter.run();  // NO
    composter.runCycle(composter.getMaxCycles());
    retValue = true;
  } else if (strcmp2(buff, (char*)"UPTIME")) {
    sprintf(summary, "# uptime %s\r\n", datecalc.timeDiff(millis()).time.c_str());
    SerialBT.print(summary);
    retValue = true;
  } else if (strcmp2(buff, (char*)"LOAD")) {
    composter.run();  // NO
    composter.stopOnDischarge(false);
    composter.stopOnLoad(true);
    retValue = true;
  } else if (strcmp2(buff, (char*)"DISCHARGE")) {
    composter.run();  // NO
    composter.stopOnLoad(false);
    composter.stopOnDischarge(true);
    retValue = true;
  } else if (strcmp2(buff, (char*)"STATUS")) {
    sprintf(summary, "#running, load, discharge, cycles\r\n");
    SerialBT.print(summary);
    sprintf(summary, "#%d, %d, %d, %d\r\n", composter.isRunning(), composter.stopOnLoad(), composter.stopOnDischarge(), composter.getRemindingCycles());
    SerialBT.print(summary);
    retValue = true;
  } else if (strcmp2(buff, (char*)"CONFIG")) {
    sprintf(summary, "#TimeOn, timeOff, maxCycles\r\n");
    SerialBT.print(summary);
    sprintf(summary, "#%02d:%02d:%02d, %02d:%02d:%02d, %d\r\n",
            composter.getTimeOn() / 3600,        // h
            composter.getTimeOn() % 3600 / 60,   // m
            composter.getTimeOn() % 60,          // s
            composter.getTimeOff() / 3600,       // h
            composter.getTimeOff() % 3600 / 60,  // m
            composter.getTimeOff() % 60,         // s
            composter.getMaxCycles());
    SerialBT.print(summary);
    retValue = true;
  } else if (stristr(buff, (char*)"CYCLES ") == buff) {
    int c = atoi(&buff[strlen("CYCLES ")]);
    composter.run();  // NO
    composter.runCycle(c);
    retValue = true;
  } else if (stristr(buff, "TIMEON ") == buff) {
    int c = atoi(&buff[strlen("TIMEON ")]);
    composter.setTimeOn(c);  // NO
    retValue = true;
  } else if (strstr(buff, "TIMEOFF ") == buff) {
    int c = atoi(&buff[strlen("TIMEOFF ")]);
    composter.setTimeOff(c);  // NO
    retValue = true;
  }
  return retValue;
}