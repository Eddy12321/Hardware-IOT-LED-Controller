#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1213");
BLECharacteristic COMMAND("19B10001-E8F2-537E-4F6C-D104768A1214",  BLERead | BLEWrite, 2);
BLECharacteristic CHECK("19B10001-E8F2-537E-4F6C-D104768A1215",  BLERead | BLEWrite, 1);

byte Command[2];
byte Check;
bool Write1 = true;
bool Write2 = false;

BLEDevice central;

void setup() {
  if (!BLE.begin()) {
    while(1);
  }
  
  BLE.setLocalName("Controller");
  BLE.setAdvertisedService(ledService);

  ledService.addCharacteristic(COMMAND);
  ledService.addCharacteristic(CHECK);

  BLE.addService(ledService);
  
  for (int i = 0; i < 2; i++) {
    Command[i] = 0;
  }

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
    Write();
  }
}

void Write() {
  CHECK.readValue(Check);
  if (Check > 0) {
    return;
  }
  Check = 100;
  COMMAND.writeValue(Command, 2);
  CHECK.writeValue(Check);
  if (Write1) {
    Command[0] = 0x11;
    Command[1] = 0;
  }
  else {
    Command[0] = 0x12;
    Command[1] = 0;
  }
  Write1 = !Write1; 
  Write2 = !Write2;
}
