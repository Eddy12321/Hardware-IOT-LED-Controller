#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLECharacteristic RED1("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite, 195);
BLECharacteristic RED2("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLEWrite, 195);
BLECharacteristic RED3("19B10001-E8F2-537E-4F6C-D104768A1217", BLERead | BLEWrite, 190);
BLECharacteristic GREEN1("19B10001-E8F2-537E-4F6C-D104768A1218", BLERead | BLEWrite, 195);
BLECharacteristic GREEN2("19B10001-E8F2-537E-4F6C-D104768A1219", BLERead | BLEWrite, 195);
BLECharacteristic GREEN3("19B10001-E8F2-537E-4F6C-D104768A1220", BLERead | BLEWrite, 190);
BLECharacteristic BLUE1("19B10001-E8F2-537E-4F6C-D104768A1221", BLERead | BLEWrite, 195);
BLECharacteristic BLUE2("19B10001-E8F2-537E-4F6C-D104768A1222", BLERead | BLEWrite, 195);
BLECharacteristic BLUE3("19B10001-E8F2-537E-4F6C-D104768A1223", BLERead | BLEWrite, 190);

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

BLEDevice central;

void setup() {

  if (!BLE.begin()) {
    while(1);
  }

  BLE.setLocalName("Controller");
  BLE.setAdvertisedService(ledService);

  ledService.addCharacteristic(RED1);
  ledService.addCharacteristic(RED2);
  ledService.addCharacteristic(RED3);
  ledService.addCharacteristic(GREEN1);
  ledService.addCharacteristic(GREEN2);
  ledService.addCharacteristic(GREEN3);
  ledService.addCharacteristic(BLUE1);
  ledService.addCharacteristic(BLUE2);
  ledService.addCharacteristic(BLUE3);

  BLE.addService(ledService);

  for (int i = 0; i < 195; i++) {
    red1[i] = 0;
    green1[i] = 0;
    blue1[i] = 0;
  }
  for (int j = 0; j < 195; j++) {
    red2[j] = 0;
    green2[j] = 0;
    blue2[j] = 0;
  }
  for (int k = 0; k < 190; k++) {
    red3[k] = 0;
    green3[k] = 0;
    blue3[k] = 0;
  }

  RED1.writeValue(red1, 195);
  RED2.writeValue(red2, 195);
  RED3.writeValue(red3, 190);
  GREEN1.writeValue(green1, 195);
  GREEN2.writeValue(green2, 195);
  GREEN3.writeValue(green3, 190);
  BLUE1.writeValue(blue1, 195);
  BLUE2.writeValue(blue2, 195);
  BLUE3.writeValue(blue3, 190);

  BLE.advertise();
}

void loop() {

  central = BLE.central();

  if (central) {
    Connected();
  }
  
}

void Connected() {
  
  while (central.connected()) {

    for (int i = 0; i < 195; i++) {
      red1[i] = 0;
      green1[i] = 0;
      blue1[i] = 150;
    }

    RED1.writeValue(red1, 195);
    RED2.writeValue(red2, 195);
    RED3.writeValue(red3, 190);
    GREEN1.writeValue(green1, 195);
    GREEN2.writeValue(green2, 195);
    GREEN3.writeValue(green3, 190);
    BLUE1.writeValue(blue1, 195);
    BLUE2.writeValue(blue2, 195);
    BLUE3.writeValue(blue3, 190);

    delay(5000);

    for (int j = 0; j < 195; j++) {
      red1[j] = 150;
      green1[j] = 0;
      blue1[j] = 0;
    }

    RED1.writeValue(red1, 195);
    RED2.writeValue(red2, 195);
    RED3.writeValue(red3, 190);
    GREEN1.writeValue(green1, 195);
    GREEN2.writeValue(green2, 195);
    GREEN3.writeValue(green3, 190);
    BLUE1.writeValue(blue1, 195);
    BLUE2.writeValue(blue2, 195);
    BLUE3.writeValue(blue3, 190);

    delay(5000);
  }
}
