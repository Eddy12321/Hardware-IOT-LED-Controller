#include <ArduinoBLE.h>
#include <SPI.h>

byte redArray[580];
byte red1[195];
byte red2[195];
byte red3[190];
byte greenArray[580];
byte green1[195];
byte green2[195];
byte green3[190];
byte blueArray[580];
byte blue1[195];
byte blue2[195];
byte blue3[190];

BLEDevice peripheral;
BLECharacteristic RED1;
BLECharacteristic RED2;
BLECharacteristic RED3;
BLECharacteristic GREEN1;
BLECharacteristic GREEN2;
BLECharacteristic GREEN3;
BLECharacteristic BLUE1;
BLECharacteristic BLUE2;
BLECharacteristic BLUE3;

void setup() {
  SPI.begin();

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  pinMode(8, INPUT);
  
  if (!BLE.begin()) {
    while(1);
  }

  for (int i = 0; i < 580; i++) {
    redArray[i] = 0;
    greenArray[i] = 0;
    blueArray[i] = 125;
  }

  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));

  digitalWrite(5, LOW);
  SPI.transfer(0x01);
  SPI.transfer(0x41);
  digitalWrite(5, HIGH);

  sendSPI();

  delay(5000);
  
  BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1214");
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
      return;
    } 
  
    RED1 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1215");
    RED2 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1216");
    RED3 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1217");
    GREEN1 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1218");
    GREEN2 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1219");
    GREEN3 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1220");
    BLUE1 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1221");
    BLUE2 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1222");
    BLUE3 = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1223");

    for (int i = 0; i < 580; i++) {
      redArray[i] = 0;
      greenArray[i] = 125;
      blueArray[i] = 0;
    }

    sendSPI();
    
    Controlled();

    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
  }
}

void sendSPI() {
  while(!digitalRead(8));
  digitalWrite(7, HIGH);
  digitalWrite(5, LOW);
  SPI.transfer(0x02);
  SPI.transfer16(0x0000);
  for (int i = 0; i < 580; i++) {
    SPI.transfer(redArray[i]);
  }
  for (int i = 0; i < 580; i++) {
    SPI.transfer(greenArray[i]);
  }
  for (int i = 0; i < 580; i++) {
    SPI.transfer(blueArray[i]);
  }
  digitalWrite(5, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delayMicroseconds(100);
  digitalWrite(6, HIGH);
}

void Controlled() {
  while (peripheral.connected()) {
    RED1.readValue(red1, 195);
    RED2.readValue(red2, 195);
    RED3.readValue(red3, 190);
    GREEN1.readValue(green1, 195);
    GREEN2.readValue(green2, 195);
    GREEN3.readValue(green3, 190);
    BLUE1.readValue(blue1, 195);
    BLUE2.readValue(blue2, 195);
    BLUE3.readValue(blue3, 190);

    for (int i = 0; i < 195; i++) {
      redArray[i] = red1[i];
      greenArray[i] = green1[i];
      blueArray[i] = blue1[i];
    }
    for (int j = 0; j < 195; j++) {
      redArray[j+195] = red2[j];
      greenArray[j+195] = green2[j];
      blueArray[j+195] = blue2[j];
    }
    for (int k = 0; k < 190; k++) {
      redArray[k+390] = red3[k];
      greenArray[k+390] = green3[k];
      blueArray[k+390] = blue3[k];
    }
    
    sendSPI();
  }
}
