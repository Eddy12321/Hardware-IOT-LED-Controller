#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1213");
BLECharacteristic READY("19B10001-E8F2-537E-4F6C-D104768A1214",  BLERead | BLEWrite, 1);
BLECharacteristic RED1("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite, 195);
BLECharacteristic RED2("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLEWrite, 195);
BLECharacteristic RED3("19B10001-E8F2-537E-4F6C-D104768A1217", BLERead | BLEWrite, 190);
BLECharacteristic GREEN1("19B10001-E8F2-537E-4F6C-D104768A1218", BLERead | BLEWrite, 195);
BLECharacteristic GREEN2("19B10001-E8F2-537E-4F6C-D104768A1219", BLERead | BLEWrite, 195);
BLECharacteristic GREEN3("19B10001-E8F2-537E-4F6C-D104768A1220", BLERead | BLEWrite, 190);
BLECharacteristic BLUE1("19B10001-E8F2-537E-4F6C-D104768A1221", BLERead | BLEWrite, 195);
BLECharacteristic BLUE2("19B10001-E8F2-537E-4F6C-D104768A1222", BLERead | BLEWrite, 195);
BLECharacteristic BLUE3("19B10001-E8F2-537E-4F6C-D104768A1223", BLERead | BLEWrite, 190);
BLECharacteristic READER("19B10001-E8F2-537E-4F6C-D104768A1224", BLERead | BLEWrite, 1);

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

BLEDevice central;

void setup() {

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);

  if (!BLE.begin()) {
    while(1);
  }

  digitalWrite(2, HIGH);

  BLE.setLocalName("Controller");
  BLE.setAdvertisedService(ledService);

  ledService.addCharacteristic(READY);
  ledService.addCharacteristic(RED1);
  ledService.addCharacteristic(RED2);
  ledService.addCharacteristic(RED3);
  ledService.addCharacteristic(GREEN1);
  ledService.addCharacteristic(GREEN2);
  ledService.addCharacteristic(GREEN3);
  ledService.addCharacteristic(BLUE1);
  ledService.addCharacteristic(BLUE2);
  ledService.addCharacteristic(BLUE3);
  ledService.addCharacteristic(READER);

  BLE.addService(ledService);
  
  READY1 = 0;
  READER1 = 0;
  for (int i = 0; i < 190; i++) {
    red1[i] = 0;
    red2[i] = 0;
    red3[i] = 0;
    green1[i] = 80;
    green2[i] = 80;
    green3[i] = 80;
    blue1[i] = 160;
    blue2[i] = 160;
    blue3[i] = 160;
  }
  for (int i = 0; i < 5; i++) {
    red1[i+190] = 0;
    red2[i+190] = 0;
    green1[i+190] = 80;
    green2[i+190] = 80;
    blue1[i+190] = 160;
    blue2[i+190] = 160;
  }

  BLE.advertise();
}

void loop() {

  central = BLE.central();

  if (central) {
    Connected();
    digitalWrite(3, LOW);
    digitalWrite(2, HIGH);
  }
  
}

void Connected() {
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);

  RED1.writeValue(red1, 195);
  RED2.writeValue(red2, 195);
  RED3.writeValue(red3, 190);
  GREEN1.writeValue(green1, 195);
  GREEN2.writeValue(green2, 195);
  GREEN3.writeValue(green3, 190);
  BLUE1.writeValue(blue1, 195);
  BLUE2.writeValue(blue2, 195);
  BLUE3.writeValue(blue3, 190);

  READY1 = 1;
  READY.writeValue(READY1);

  for (int i = 0; i < 190; i++) {
    red1[i]++;
    red2[i]++;
    red3[i]++;
    green1[i]++;
    green2[i]++;
    green3[i]++;
    blue1[i]++;
    blue2[i]++;
    blue3[i]++;
  }
  for (int i = 0; i < 5; i++) {
    red1[i+190]++;
    red2[i+190]++;
    green1[i+190]++;
    green2[i+190]++;
    blue1[i+190]++;
    blue2[i+190]++;
  }
  
  while (central.connected()) {
    Write();
  }
}

void Write() {
  READER.readValue(READER1);

  if (READER1 != 1) {
    return;
  }
  READER1 = 0;
  READER.writeValue(READER1);

  RED1.writeValue(red1, 195);
  RED2.writeValue(red2, 195);
  RED3.writeValue(red3, 190);
  GREEN1.writeValue(green1, 195);
  GREEN2.writeValue(green2, 195);
  GREEN3.writeValue(green3, 190);
  BLUE1.writeValue(blue1, 195);
  BLUE2.writeValue(blue2, 195);
  BLUE3.writeValue(blue3, 190);

  READY1 = 1;
  READY.writeValue(READY1);

  for (int i = 0; i < 190; i++) {
    red1[i]++;
    red2[i]++;
    red3[i]++;
    green1[i]++;
    green2[i]++;
    green3[i]++;
    blue1[i]++;
    blue2[i]++;
    blue3[i]++;
  }
  for (int i = 0; i < 5; i++) {
    red1[i+190]++;
    red2[i+190]++;
    green1[i+190]++;
    green2[i+190]++;
    blue1[i+190]++;
    blue2[i+190]++;
  }
}
