#include <ArduinoBLE.h>

BLEDevice peripheral;
BLECharacteristic CHECK;
BLECharacteristic COMMAND;

byte Check = 0;
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
    CHECK.subscribe();
    CHECK.setEventHandler(BLEWritten, Read);
    
    Serial.println("Connected");

    Controlled();

    Serial.println("Resume Search");
    
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1213");

    peripheral = BLE.available();
  }
}

void Controlled() {
  while (peripheral.connected()) {
    
  }
}

void Read(BLEDevice central, BLECharacteristic characteristic) {
  Serial.println("Command");
  COMMAND.readValue(Command, 2);
  switch (Command[0]) {
    case 3:
      Serial.println("Increase blink speed");
      break;
    case 4:
      Serial.println("Decrease blink speed");
      break;
    case 11:
      Serial.println("Increase move speed");
      break;
    case 12:
      Serial.println("Decrease move speed");
      break;
    case 13:
      Serial.print("Preset: ");
      Serial.println(Command[1]);
      break;
    default:
      Serial.println("Do nothing");
      break;
  }
}
