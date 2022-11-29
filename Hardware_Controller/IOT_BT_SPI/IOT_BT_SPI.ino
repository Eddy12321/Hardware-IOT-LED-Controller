#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1213");
BLECharacteristic READY("19B10001-E8F2-537E-4F6C-D104768A1214",  BLERead | BLEWrite, 1);
BLECharacteristic RED1("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite, 20);
BLECharacteristic RED2("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLEWrite, 20);
BLECharacteristic RED3("19B10001-E8F2-537E-4F6C-D104768A1217", BLERead | BLEWrite, 20);
BLECharacteristic GREEN1("19B10001-E8F2-537E-4F6C-D104768A1218", BLERead | BLEWrite, 20);
BLECharacteristic GREEN2("19B10001-E8F2-537E-4F6C-D104768A1219", BLERead | BLEWrite, 20);
BLECharacteristic GREEN3("19B10001-E8F2-537E-4F6C-D104768A1220", BLERead | BLEWrite, 20);
BLECharacteristic BLUE1("19B10001-E8F2-537E-4F6C-D104768A1221", BLERead | BLEWrite, 20);
BLECharacteristic BLUE2("19B10001-E8F2-537E-4F6C-D104768A1222", BLERead | BLEWrite, 20);
BLECharacteristic BLUE3("19B10001-E8F2-537E-4F6C-D104768A1223", BLERead | BLEWrite, 20);
BLECharacteristic READER("19B10001-E8F2-537E-4F6C-D104768A1224", BLERead | BLEWrite, 1);

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
bool Write1 = true;
bool Write2 = false;
int i = 0;

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
  for (int i = 0; i < 20; i++) {
    red1[i] = 150;
    red2[i] = 150;
    red3[i] = 150;
    green1[i] = 0;
    green2[i] = 0;
    green3[i] = 0;
    blue1[i] = 0;
    blue2[i] = 0;
    blue3[i] = 0;
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

  RED1.writeValue(red1, 20);
  RED2.writeValue(red2, 20);
  RED3.writeValue(red3, 20);
  GREEN1.writeValue(green1, 20);
  GREEN2.writeValue(green2, 20);
  GREEN3.writeValue(green3, 20);
  BLUE1.writeValue(blue1, 20);
  BLUE2.writeValue(blue2, 20);
  BLUE3.writeValue(blue3, 20);

  READY1 = 1;
  READY.writeValue(READY1);
  
  while (central.connected()) {
    if (Write1) {
      if (i < 20) {
        red1[i] = 0;
        green1[i] = 150;
      }
      else if (i < 40) {
        red2[i - 20] = 0;
        green2[i - 20] = 150;
      }
      else {
        red3[i - 40] = 0;
        green3[i - 40] = 150;
      }
      if (i == 0) {
        red3[19] = 150;
        green3[19] = 0;
        i++;
      }
      else if (i == 59) {
        red3[18] = 150;
        green3[18] = 0;
        i = 0;
      }
      else {
        if (i < 21) {
          red1[i-1] = 150;
          green1[i-1] = 0;
        }
        else if (i < 41) {
          red2[i-21] = 150;
          green2[i-21] = 0;
        }
        else {
          red3[i-41] = 150;
          green3[i-41] = 0;        
        }
        i++;
      }
      Write1 = !Write1;
      Write2 = !Write2;
    }
    if (Write2) {
      Write();
    }
  }
}

void Write() {
  READER.readValue(READER1);

  if (READER1 != 1) {
    return;
  }
  READER1 = 0;
  READER.writeValue(READER1);

  RED1.writeValue(red1, 20);
  RED2.writeValue(red2, 20);
  RED3.writeValue(red3, 20);
  GREEN1.writeValue(green1, 20);
  GREEN2.writeValue(green2, 20);
  GREEN3.writeValue(green3, 20);
  BLUE1.writeValue(blue1, 20);
  BLUE2.writeValue(blue2, 20);
  BLUE3.writeValue(blue3, 20);

  READY1 = 1;
  READY.writeValue(READY1);

  Write1 = !Write1;
  Write2 = !Write2;
}
