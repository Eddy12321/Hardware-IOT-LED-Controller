#include <ArduinoBLE.h>

BLEDevice peripheral;
BLECharacteristic CHECK;
BLECharacteristic COMMAND;

byte Check;
byte Command[2];

void setup() {
  Serial.begin(115200);

  Serial.println("Begin");

  if (!BLE.begin()) {
    while(1);
  }

  BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1213");
}

void loop() {
  peripheral = BLE.available();

  if (peripheral) {

    Serial.println("Here");

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

    COMMAND = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
    CHECK = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1215");

    Serial.println("Connected");
    
    Controlled();
    
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1213");

    peripheral = BLE.available();
  }
}

void Controlled() {
  while (peripheral.connected()) {
    CHECK.readValue(Check);
    if (Check > 0) {
      COMMAND.readValue(Command, 2);
      for (int i = 0; i < 2; i++) {
        Serial.println(Command[i]);
      }
      Check = 0;
      CHECK.writeValue(Check);
    }
  }
}
