#include <ArduinoBLE.h>

byte redArray[6];
byte red1[2];
byte red2[2];
byte red3[2];
byte greenArray[6];
byte green1[2];
byte green2[2];
byte green3[2];
byte blueArray[6];
byte blue1[2];
byte blue2[2];
byte blue3[2];

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

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);

  if (!BLE.begin()) {
    while(1);
  }

  Serial.begin(115200);

  digitalWrite(2, HIGH);

  for (int i = 0; i < 6; i++) {
    redArray[i] = 0;
    greenArray[i] = 0;
    blueArray[i] = 0;
  }

  delay(5000);

  Serial.println("Start");
  
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
      BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
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

    RED1.subscribe();
    RED1.setEventHandler(BLEWritten, Read);
    
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

  }
}

void Read(BLEDevice central, BLECharacteristic characteristic) {
  RED1.readValue(red1, 2);
  RED2.readValue(red2, 2);
  RED3.readValue(red3, 2);
  GREEN1.readValue(green1, 2);
  GREEN2.readValue(green2, 2);
  GREEN3.readValue(green3, 2);
  BLUE1.readValue(blue1, 2);
  BLUE2.readValue(blue2, 2);
  BLUE3.readValue(blue3, 2);

  for (int i = 0; i < 2; i++) {
      redArray[i] = red1[i];
      redArray[i+2] = red2[i];
      redArray[i+4] = red3[i];
      greenArray[i] = green1[i];
      greenArray[i+2] = green2[i];
      greenArray[i+4] = green3[i];
      blueArray[i] = blue1[i];
      blueArray[i+2] = blue2[i];
      blueArray[i+4] = blue3[i];
  }
  Serial.println("Reds");
  for (int i = 0; i < 6; i++) {
    Serial.println(redArray[i]);
  }
  Serial.println("Greens");
  for (int i = 0; i < 6; i++) {
    Serial.println(greenArray[i]);
  }
  Serial.println("Blues");
  for (int i = 0; i < 6; i++) {
    Serial.println(blueArray[i]);
  }

}
