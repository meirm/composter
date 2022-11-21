#define soilPin 2
#define thermistorPin 16
#define thermistorOut 17

int refres = 10000;
float nominalres = 100000.0;
int nominaltemp = 25;
int bcoef = 3950;

int soilMoisture;

void setup() {
  pinMode(soilPin, INPUT);
  pinMode(thermistorPin, OUTPUT);
  pinMode(thermistorOut, INPUT);
  Serial.begin(115200);
}

void loop() {
  // Spil moisture reading
  soilMoisture = analogRead(soilPin);
  Serial.print(soilMoisture);
  soilMoisture = map(soilMoisture, 4095, 0, 0, 100);
  Serial.print("Moisture: ");
  Serial.println(soilMoisture);

  //temperature reading
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
  Serial.print("Temperature: ");
  Serial.println(temp2);


  delay(2000);
}
