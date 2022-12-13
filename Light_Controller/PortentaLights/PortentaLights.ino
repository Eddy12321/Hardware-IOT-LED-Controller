#include <ArduinoBLE.h>

BLEDevice peripheral;
BLECharacteristic CHECK;
BLECharacteristic COMMAND;

uint8_t leds[180];
byte Command[2];
uint8_t tempArray[180];
byte Check;
int blinkSpeed = 0;
int movementSpeed = 0;
int i;
int j;
int k;
byte com;
byte led;
byte sendByte;

void setup() {

  Serial.begin(115200);

  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  
  Serial.println("Begin");

  if (!BLE.begin()) {
    while(1);
  }

  for (i = 0; i < 180; i+=3) {
    leds[i] = 150;
    leds[i+1] = 0;
    leds[i+2] = 0;
  }
  
  render();

  delay(5000);

  for (i = 0; i < 180; i++) {
    leds[i] = 0;
  }

  render();

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
      Check = 0;
      CHECK.writeValue(Check);
      Execute();
    }   
    LightUp();
  }
}

void LightUp() {

  if (movementSpeed > 0) {
    for (i = 0; i < 180; i++) {
      tempArray[i] = leds[i];
    }
    leds[0] = tempArray[179];
    for (i = 1; i < 180; i++) {
      leds[i] = tempArray[i-1];
    }
    delay(map(movementSpeed, 0, 1024, 1024, 0));
  }
  if (blinkSpeed > 0) {
    for (i = 0; i < 180; i++) {
      tempArray[i] = leds[i];
      leds[i] = 0;
    }
    render();
    for (i = 0; i < 180; i++) {
      leds[i] = tempArray[i];   
    }
    delay(map(blinkSpeed, 0, 1024, 1024, 0));
  }

  noInterrupts();
  render();
 
}


void render() {
  for (i = 0; i < 180; i++) {
    sendByte = leds[i];
    for (j = 0; j < 8; j++) {
      if (sendByte >> j && 0x01) {
        digitalWrite(14, HIGH);
        for (k = 0; k < 150; k++) {
          asm volatile("nop\n\t");
        }
        digitalWrite(14, LOW);
        for (k = 0; k < 50; k++) {
          asm volatile("nop\n\t");
        }
      }
      else {
        digitalWrite(14, HIGH);
        for (k = 0; k < 50; k++) {
          asm volatile("nop\n\t");
        }
        digitalWrite(14, LOW); 
        for (k = 0; k < 150; k++) {
          asm volatile("nop\n\t");
        }
      }
    }
  }
  interrupts();
  delay(10);
}

void Execute() {
  com = Command[0];
  led = Command[1];
  if (com < 16) {
    switch (com) {
      case 1:
        for (i = 0; i < 180; i++) {
          if (leds[i] > 0 & leds[i] < 255) {
            leds[i]++;
          }
        }
        break;
      case 2:
        for (i = 0; i < 180; i++) {
          if (leds[i] > 0) {
            leds[i]--;
          }
        }
        break;
      case 3:
        if (blinkSpeed < 1024) {
          blinkSpeed++;
        }
        break;
      case 4:
        if (blinkSpeed > 0) {
          blinkSpeed--;
        }
        break;
      case 5:
        if (leds[led] < 255) {
          leds[led]++;
        }
        break;
      case 6:
        if (leds[led] > 0) {
          leds[led]--;
        }
        break;
      case 7:
        if (leds[led+1] < 255) {
          leds[led+1]++;
        }
        break;
      case 8:
        if (leds[led+1] > 0) {
          leds[led+1]--;
        }
        break;
      case 9:
        if (leds[led+2] < 255) {
          leds[led+2]++;
        }
        break;
      case 10:
        if (leds[led+2] > 0) {
          leds[led+2]--;
        }
        break;
      case 11:
        if (movementSpeed < 1024) {
          movementSpeed++;
        }
        break;
      case 12:
        if (movementSpeed > 0) {
          movementSpeed--;
        }
        break;
    }
  }
  else if (com > 16 & com < 32) {
    switch (com-16) {
      case 0:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 150;
          leds[i+1] = 0;
          leds[i+2] = 0;
        }
        movementSpeed = 0;
        blinkSpeed = 0;
        break;
      case 1:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 0;
          leds[i+1] = 150;
          leds[i+2] = 0;
        }
        movementSpeed = 0;
        blinkSpeed = 0;
        break;
      case 2:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 0;
          leds[i+1] = 0;
          leds[i+2] = 150;
        }
        movementSpeed = 0;
        blinkSpeed = 0;
        break;
      case 3:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 150;
          leds[i+1] = 150;
          leds[i+2] = 0;
        }
        movementSpeed = 0;
        blinkSpeed = 0;
        break;
      case 4:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 0;
          leds[i+1] = 150;
          leds[i+2] = 150;
        }
        movementSpeed = 0;
        blinkSpeed = 0;      
        break;
      case 5:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 150;
          leds[i+1] = 0;
          leds[i+2] = 150;
        }
        movementSpeed = 0;
        blinkSpeed = 0;      
        break;
      case 6:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 150;
          leds[i+1] = 150;
          leds[i+2] = 150;
        }
        movementSpeed = 0;
        blinkSpeed = 0;      
        break;
      case 7:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 200;
          leds[i+1] = 100;
          leds[i+2] = 100;
        }
        movementSpeed = 0;
        blinkSpeed = 0;      
        break;
      case 8:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 100;
          leds[i+1] = 200;
          leds[i+2] = 100;
        }
        movementSpeed = 0;
        blinkSpeed = 0;      
        break;
      case 9:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 100;
          leds[i+1] = 100;
          leds[i+2] = 200;
        }
        movementSpeed = 0;
        blinkSpeed = 0;      
        break;
      case 10:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 50;
          leds[i+1] = 100;
          leds[i+2] = 150;
        }
        movementSpeed = 0;
        blinkSpeed = 0;      
        break;
      case 11:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 100;
          leds[i+1] = 150;
          leds[i+2] = 50;
        }
        movementSpeed = 0;
        blinkSpeed = 0;      
        break;
      case 12:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 150;
          leds[i+1] = 50;
          leds[i+2] = 100;
        }
        movementSpeed = 0;
        blinkSpeed = 0;      
        break;
      case 13:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 0;
          leds[i+1] = 0;
          leds[i+2] = 0;
        }
        movementSpeed = 0;
        blinkSpeed = 0;      
        break;
      case 14:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 0;
          leds[i+1] = 0;
          leds[i+2] = 0;
        }
        movementSpeed = 0;
        blinkSpeed = 0;      
        break;
      case 15:
        for (i = 0; i < 180; i+=3) {
          leds[i] = 0;
          leds[i+1] = 0;
          leds[i+2] = 0;
        }
        movementSpeed = 0;
        blinkSpeed = 0;      
        break;
    }
  }
  else if (com >= 32) {
    switch (com-32) {
      case 0:
        break;
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        break;
      case 7:
        break;
      case 8:
        break;
      case 9:
        break;
      case 10:
        break;
      case 11:
        break;
      case 12:
        break;
      case 13:
        break;
      case 14:
        break;
      case 15:
        break;
    }
  }
}
