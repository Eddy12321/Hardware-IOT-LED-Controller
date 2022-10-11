#include <SPI.h>

byte redArray[20];
byte greenArray[20];
byte blueArray[20];
int finish = 1;
bool Avail = false;
bool Sent = false;
bool Ready = true;

void setup() {

  SPI.begin();
  
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  pinMode(8, INPUT);

  for (int i = 0; i < 20; i++) {
    redArray[i] = 0;
    greenArray[i] = 0;
    blueArray[i] = 0;
  }

  SPI.beginTransaction(SPISettings(6000000, MSBFIRST, SPI_MODE0));

  Write();
  
}

void loop() {

  if (finish%2 == 1) {
    for (int i = 0; i < 20; i++) {
      redArray[i] = 170;
      greenArray[i] = 0;
      blueArray[i] = 170;
    }  
  }
  else {
    for (int i = 0; i < 20; i++) {
      redArray[i] = 0;
      greenArray[i] = 170;
      blueArray[i] = 0;
    }
  }

  if (Ready == true) {
    CheckAvail();
  }
  if (Avail == true) {
    Write();
  }
  if (Sent == true) {
    CheckSent();
  }
  
}

void Write() {
  Avail = false;
  digitalWrite(7, HIGH);
  digitalWrite(5, LOW);
  SPI.transfer(0x06);
  digitalWrite(5, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(5, LOW);
  SPI.transfer(0x02);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  for (int i = 0; i < 20; i++) {
    SPI.transfer(redArray[i]);
  }
  for (int j = 0; j < 20; j++) {
    SPI.transfer(greenArray[j]);
  }
  for (int k = 0; k < 20; k++) {
    SPI.transfer(blueArray[k]);
  }
  digitalWrite(5, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(3, LOW);
  digitalWrite(6, LOW);
  Sent = true;
}

void CheckAvail() {
  if (digitalRead(8)) {
    return;
  }
  Ready = false;
  digitalWrite(3, HIGH);
  Avail = true;
}

void CheckSent() {
  if (!digitalRead(4)) {
    return;
  }
  digitalWrite(6, HIGH);
  finish++;
  Sent = false;
  Ready = true;
}
