#include <SPI.h>

byte redArray[580];
byte greenArray[580];
byte blueArray[580];

void setup() {

  SPI.begin();

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  pinMode(8, INPUT);

  for (int i = 0; i < 580; i++) {
    redArray[i] = 0;
    greenArray[i] = 0;
    blueArray[i] = 0;
  }

  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));

  digitalWrite(5, LOW);
  SPI.transfer(0x01);
  SPI.transfer(0x41);
  digitalWrite(5, HIGH);

  Write();

  delay(5000);
  
}

void loop() {
  for (int i = 0; i < 20; i++) {
    redArray[i] = 170;
    greenArray[i] = 0;
    blueArray[i] = 170;
  }

  Write();

  delay(75);

  for (int i = 0; i < 20; i++) {
    redArray[i] = 0;
    greenArray[i] = 170;
    blueArray[i] = 0;
  }

  Write();

  delay(75);
}

void Write() {
  while (!digitalRead(8));
  digitalWrite(7, HIGH);
  digitalWrite(5, LOW);
  SPI.transfer(0x02);
  SPI.transfer16(0x0000);
  for (int i = 0; i < 580; i++) {
    SPI.transfer(redArray[i]);
  }
  for (int j = 0; j < 580; j++) {
    SPI.transfer(greenArray[j]);
  }
  for (int k = 0; k < 580; k++) {
    SPI.transfer(blueArray[k]);
  }
  digitalWrite(5, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delayMicroseconds(100);
  digitalWrite(6, HIGH);
}
