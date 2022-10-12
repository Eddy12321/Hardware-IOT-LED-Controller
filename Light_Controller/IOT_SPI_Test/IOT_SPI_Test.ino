#include <ArduinoBLE.h>
#include <SPI.h>

BLEDevice peripheral;
BLECharacteristic READY;
BLECharacteristic RED1;
BLECharacteristic RED2;
BLECharacteristic RED3;
BLECharacteristic GREEN1;
BLECharacteristic GREEN2;
BLECharacteristic GREEN3;
BLECharacteristic BLUE1;
BLECharacteristic BLUE2;
BLECharacteristic BLUE3;
BLECharacteristic READER;
bool Written = false;
bool Received = true;
byte READY1;
byte redArray[60];
byte red1[20];
byte red2[20];
byte red3[20];
byte greenArray[60];
byte green1[20];
byte green2[20];
byte green3[20];
byte blueArray[60];
byte blue1[20];
byte blue2[20];
byte blue3[20];
byte READER1;

void setup() {

  SPI.begin();
  
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);

  if (!BLE.begin()) {
    while(1);
  }

  READY1 = 0;
  READER1 = 0;
  for (int i = 0; i < 60; i++) {
    redArray[i] = 0;
    greenArray[i] = 0;
    blueArray[i] = 0;
  }
  
  BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1213");

  SPI.beginTransaction(SPISettings(6000000, MSBFIRST, SPI_MODE0));

}

void loop() {

  peripheral = BLE.available();

  if (peripheral) {

    if (peripheral.localName() != "Controller") {
      return;
    }

    BLE.stopScan();

    if (!(peripheral.connect())) {
      return;
    }
  
    if (!(peripheral.discoverAttributes())) {
      peripheral.disconnect();
      BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1213");
      return;
    } 

    READY = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
    RED1 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1215");
    RED2 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1216");
    RED3 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1217");
    GREEN1 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1218");
    GREEN2 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1219");
    GREEN3 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1220");
    BLUE1 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1221");
    BLUE2 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1222");
    BLUE3 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1223");
    READER = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1224");
    
    Controlled();
    
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");

    peripheral = BLE.available();
  }
}

void Controlled() {
  while (peripheral.connected()) {
    if (Received) {
      Read();
    }
    if (Written) {
      Write();
    }
  }
}

void Read() {
  READY.readValue(READY1);

  if (READY1 != 1) {
    return;
  }

  READY1 = 0;
  READY.writeValue(READY1);
  
  RED1.readValue(red1, 20);
  RED2.readValue(red2, 20);
  RED3.readValue(red3, 20);
  GREEN1.readValue(green1, 20);
  GREEN2.readValue(green2, 20);
  GREEN3.readValue(green3, 20);
  BLUE1.readValue(blue1, 20);
  BLUE2.readValue(blue2, 20);
  BLUE3.readValue(blue3, 20);

  for (int i = 0; i < 20; i++) {
    redArray[i] = red1[i];
    redArray[i+20] = red2[i];
    redArray[i+40] = red3[i];
    greenArray[i] = green1[i];
    greenArray[i+20] = green2[i];
    greenArray[i+40] = green3[i];
    blueArray[i] = blue1[i];
    blueArray[i+20] = blue2[i];
    blueArray[i+40] = blue3[i];
  }

  READER1 = 1;
  READER.writeValue(READER1);
  Received = false;
  Written = true;
}

void Write() {
  if (!digitalRead(4)) {
    return;
  }
  digitalWrite(3, HIGH);
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
  for (int i = 0; i < 60; i++) {
    SPI.transfer(redArray[i]);
  }
  for (int j = 0; j < 60; j++) {
    SPI.transfer(greenArray[j]);
  }
  for (int k = 0; k < 60; k++) {
    SPI.transfer(blueArray[k]);
  }
  digitalWrite(5, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(3, LOW);
  Written = false;
  Received = true;
}
