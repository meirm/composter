#include "config.h"
#if USESDCARD == YES
#include <SPI.h>
#include <SD.h>
#endif
#include "DHT.h"  // temp and humidity sensor
#define DHTTYPE DHT11

//RTC
#include <ThreeWire.h>
#include <RtcDS1302.h>
ThreeWire myWire(IO, SCLK, CE);
RtcDS1302<ThreeWire> Rtc(myWire);

//Thermistor

File myFile;
const int chipSelect = 10;  //CS on Card reader
// CS 10
// SCK 13
// MOSI 11
// MISO 12

// Soil moisture A0
const int soilPin = A0;  // input from soil humidity sensor
int soilMoisture;

const int DHTPIN = 3;  // input from DHT humidity and temperature sensor
DHT dht(DHTPIN, DHTTYPE);
int humi;
float tempC;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Initializing SD card . . .  ");

  pinMode(thermistorPin, OUTPUT);
#if USESDCARD == YES
  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
#endif
  dht.begin();  // initilize DHT temp and humi sensor


  Rtc.Begin();  // initilize RTC DS 1302
  RtcDateTime compiledTime = RtcDateTime(__DATE__, __TIME__);
  Serial.print(__DATE__);
  Serial.println(__TIME__);


  // RTC issues
  if (!Rtc.IsDateTimeValid()) {
    Serial.print("RTC lost confidence in the DateTime");
    //Rtc.SetDateTime(compiledTime);  // set RTC time to current PC time
  }
  if (!Rtc.GetIsRunning()) {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }
  /*if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }  */


  // INITILIZE SD CARD
  myFile = SD.open("log.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("Writing to log.txt");
    myFile.print("Initilization done.");
    myFile.close();
  } else {
    Serial.println("Error opening file");
  }
  delay(1000);


  //  RTC check time
  RtcDateTime now = Rtc.GetDateTime();
  myFile = SD.open("log.txt", FILE_WRITE);
  if (now < compiledTime) {
    Serial.println("RTC is older than compiled time");
    myFile.println(" Time is not logged properly.");
    myFile.close();
  } else {
    Serial.println("RTC is newer or same as compiled");
    myFile.println(" Time logged properly.");
    myFile.close();
  }
}


void loop() {
  RtcDateTime now = Rtc.GetDateTime();  // get datetime

  //get readings
  soilMoisture = analogRead(soilPin);
  soilMoisture = map(soilMoisture, 1023, 0, 0, 100);
  humi = dht.readHumidity();
  tempC = dht.readTemperature();

  digitalWrite(thermistorPin, HIGH);
  float temp2read;
  temp2read = analogRead(thermistorOut);
  digitalWrite(thermistorPin, LOW);  // to prevent overheating / flase readings
  temp2read = refres / (1023.0 / temp2read - 1);

  float temp2;
  //convert thermistor reading to deg C
  temp2 = temp2read / nominalres;
  temp2 = log(temp2);
  temp2 = temp2 / bcoef;
  temp2 += 1.0 / (nominaltemp + 273.15);
  temp2 = 1.0 / temp2;
  temp2 -= 273.15;


  printDateTime(now);
  Serial.print("   Moisture: ");
  Serial.print(soilMoisture);
  Serial.print("  Humidity: ");
  Serial.print(humi);
  Serial.print("  Temperature: ");
  Serial.print(tempC);
  Serial.print("  Thermistor temp: ");
  Serial.println(temp2);
  myFile = SD.open("log.txt", FILE_WRITE);
  if (myFile) {
    printDateTimeFile(now);
    myFile.print("   Soil moisture: ");
    myFile.print(soilMoisture);
    myFile.print("%  Temperature 1: ");
    myFile.print(tempC);
    myFile.print(char(176));
    myFile.print("C  Temperature 2: ");
    myFile.print(temp2);
    myFile.print(char(176));
    myFile.print("C  Humidity: ");
    myFile.print(humi);
    myFile.println("%");
    myFile.close();
  } else {
    Serial.println("Error writing to file");
  }
  delay(2000);
}


#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime& dt) {
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  Serial.print(datestring);
}

void printDateTimeFile(const RtcDateTime& dt) {
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  myFile.print(datestring);
}