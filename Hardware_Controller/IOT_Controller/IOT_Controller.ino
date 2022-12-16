#include <ArduinoBLE.h>
#include <PCF8574.h>

#define INTERRUPTMOVEBLINK 2

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1213");
BLECharacteristic COMMAND("19B10001-E8F2-537E-4F6C-D104768A1214",  BLERead | BLEWrite, 2);
BLECharacteristic CHECK("19B10001-E8F2-537E-4F6C-D104768A1215",  BLERead | BLEWrite | BLENotify, 1);

byte Command[2] = {0, 0};
byte Check = 0;
volatile bool Ready = false;

int buttonPin = P2;
int MovePlus = P7;
int MoveMinus = P6;
int BlinkPlus = P5;
int BlinkMinus = P4;
int Preset1 = P4;
int Preset2 = P5;
int Preset3 = P6;
int Preset4 = P7;

volatile uint8_t Move;
volatile uint8_t Blink;
volatile uint8_t Button;
volatile uint8_t MovePlusLast = LOW;
volatile uint8_t BlinkPlusLast = LOW;
volatile uint8_t ButtonLast = LOW;
volatile uint8_t PresetChoice = 0;

void UpdateMoveBlink();

PCF8574::DigitalInput ReadMoveBlink;
PCF8574::DigitalInput ReadPreset;

PCF8574 MoveBlink(0x3C, INTERRUPTMOVEBLINK, UpdateMoveBlink);
PCF8574 Preset(0x38);

BLEDevice central;

void setup() {
  Serial.begin(115200);

  delay(5000);
  
  if (!BLE.begin()) {
    while(1);
  }

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  pinMode(15, OUTPUT);
  digitalWrite(15, LOW);

  MoveBlink.pinMode(buttonPin, INPUT_PULLUP);
  MoveBlink.pinMode(MovePlus, INPUT_PULLUP);
  MoveBlink.pinMode(MoveMinus, INPUT_PULLUP);
  MoveBlink.pinMode(BlinkPlus, INPUT_PULLUP);
  MoveBlink.pinMode(BlinkMinus, INPUT_PULLUP);
  Preset.pinMode(Preset1, INPUT_PULLUP);
  Preset.pinMode(Preset2, INPUT_PULLUP);
  Preset.pinMode(Preset3, INPUT_PULLUP);
  Preset.pinMode(Preset4, INPUT_PULLUP);

  MoveBlink.setLatency(0);

  MoveBlink.begin();

  Serial.println("Begin");
  
  BLE.setLocalName("Controller");
  BLE.setAdvertisedService(ledService);

  ledService.addCharacteristic(COMMAND);
  ledService.addCharacteristic(CHECK);

  BLE.addService(ledService);

  BLE.advertise();

  Serial.println("GO");
}

void loop() {
  central = BLE.central();
  if (central) {
    Serial.println("Connected");
    digitalWrite(13, HIGH);
    digitalWrite(14, LOW);
    digitalWrite(15, LOW);
    Connected();
  }
}

void Connected() {
  attachInterrupt(digitalPinToInterrupt(INTERRUPTMOVEBLINK), UpdateMoveBlink, FALLING);
  while (central.connected()) {
    if (Ready) {
      detachInterrupt(digitalPinToInterrupt(INTERRUPTMOVEBLINK));
      digitalWrite(13, LOW);
      digitalWrite(14, HIGH);
      digitalWrite(15, LOW);
      Write();
      digitalWrite(13, HIGH);
      digitalWrite(14, LOW);
      digitalWrite(15, LOW);
    }
  }
  Serial.println("Waiting for connection");
}

void Write() {
  if (!CHECK.subscribed()) {
    return;
  }
  Check++;
  COMMAND.writeValue(Command, 2);
  CHECK.writeValue(Check);
  Ready = false;
  Serial.println("Send");
  attachInterrupt(digitalPinToInterrupt(INTERRUPTMOVEBLINK), UpdateMoveBlink, FALLING);
}

void UpdateMoveBlink() {
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
  digitalWrite(15, HIGH);
  ReadMoveBlink = MoveBlink.digitalReadAll();
  Move = ReadMoveBlink.p7;
  Blink = ReadMoveBlink.p5;
  Button = ReadMoveBlink.p2;
  if ((MovePlusLast == LOW) && (Move == HIGH)) {
    if (ReadMoveBlink.p6 == LOW) {
      Command[0] = 0x0C;
      Command[1] = 0;
      Ready = true;
      MovePlusLast = Move;
      return;
    } else {
      Command[0] = 0x0B;
      Command[1] = 0;
      Ready = true;
      MovePlusLast = Move;
      return;
    }
  }
  MovePlusLast = Move;
  if ((BlinkPlusLast == LOW) && (Blink == HIGH)) {
    if (ReadMoveBlink.p4 == LOW) {
      Command[0] = 0x04;
      Command[1] = 0;
      Ready = true;
      BlinkPlusLast = Blink;
      return;
    } else {
      Command[0] = 0x03;
      Command[1] = 0;
      Ready = true;
      BlinkPlusLast = Blink;
      return;
    }
  }
  BlinkPlusLast = Blink;
  if ((Button == LOW) && (ButtonLast == HIGH)) {
    ReadPreset = Preset.digitalReadAll();
    if ((!ReadPreset.p4) && (!ReadPreset.p5) && (!ReadPreset.p6) && (!ReadPreset.p7)) {
      Command[0] = 0;
      Command[1] = 0;
      Ready = true;
    }
    else {
      PresetChoice = !ReadPreset.p4 * 1 + !ReadPreset.p5 * 2 
                  + !ReadPreset.p6 * 4 + !ReadPreset.p7 * 8;
      Command[0] = 0x0D;
      Command[1] = PresetChoice;
      Ready = true;
    }
  } 
  ButtonLast = Button;
}
