#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLECharacteristic RED1("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite | BLEIndicate, 2);
BLECharacteristic RED2("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLEWrite, 2);
BLECharacteristic RED3("19B10001-E8F2-537E-4F6C-D104768A1217", BLERead | BLEWrite, 2);
BLECharacteristic GREEN1("19B10001-E8F2-537E-4F6C-D104768A1218", BLERead | BLEWrite, 2);
BLECharacteristic GREEN2("19B10001-E8F2-537E-4F6C-D104768A1219", BLERead | BLEWrite, 2);
BLECharacteristic GREEN3("19B10001-E8F2-537E-4F6C-D104768A1220", BLERead | BLEWrite, 2);
BLECharacteristic BLUE1("19B10001-E8F2-537E-4F6C-D104768A1221", BLERead | BLEWrite, 2);
BLECharacteristic BLUE2("19B10001-E8F2-537E-4F6C-D104768A1222", BLERead | BLEWrite, 2);
BLECharacteristic BLUE3("19B10001-E8F2-537E-4F6C-D104768A1223", BLERead | BLEWrite, 2);

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
unsigned long sendtime = 0;

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

  for (int i = 0; i < 2; i++) {
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

  while (central.connected()) {

//  while ((millis() - sendtime) < 1000);
//  sendtime = millis();

    for (int i = 0; i < 2; i++) {
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

    
    RED2.writeValue(red2, 2);
    RED3.writeValue(red3, 2);
    GREEN1.writeValue(green1, 2);
    GREEN2.writeValue(green2, 2);
    GREEN3.writeValue(green3, 2);
    BLUE1.writeValue(blue1, 2);
    BLUE2.writeValue(blue2, 2);
    BLUE3.writeValue(blue3, 2);
    RED1.writeValue(red1, 2);

  }
}
