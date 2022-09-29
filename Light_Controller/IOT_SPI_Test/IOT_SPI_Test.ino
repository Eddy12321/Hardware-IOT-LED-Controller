#include <ArduinoBLE.h>

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

byte READY1;
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
byte READER1;

void setup() {

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);

  if (!BLE.begin()) {
    while(1);
  }

  Serial.begin(115200);

  digitalWrite(2, HIGH);

  READY1 = 0;
  READER1 = 0;
  for (int i = 0; i < 580; i++) {
    redArray[i] = 0;
    greenArray[i] = 0;
    blueArray[i] = 0;
  }


  Serial.println("Start");
  
  BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1213");
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

    digitalWrite(3, LOW);
    digitalWrite(2, HIGH);

    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");

    peripheral = BLE.available();
  }
}

void Controlled() {
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  while (peripheral.connected()) {
    Read();
  }
}

void Read() {
  READY.readValue(READY1);

  if (READY1 != 1) {
    return;
  }

  READY1 = 0;
  READY.writeValue(READY1);
  
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
    redArray[i+195] = red2[i];
    greenArray[i] = green1[i];
    greenArray[i+195] = green2[i];
    blueArray[i] = blue1[i];
    blueArray[i+195] = blue2[i];
  }

  for (int i = 0; i < 190; i++) {
    redArray[i+390] = red3[i];
    greenArray[i+390] = green3[i];
    blueArray[i+390] = blue3[i];
  }
  
  Serial.println("Reds");
  for (int i = 0; i < 580; i++) {
    Serial.println(redArray[i]);
  }
  Serial.println("Greens");
  for (int i = 0; i < 580; i++) {
    Serial.println(greenArray[i]);
  }
  Serial.println("Blues");
  for (int i = 0; i < 580; i++) {
    Serial.println(blueArray[i]);
  }

  READER1 = 1;
  READER.writeValue(READER1);
}
