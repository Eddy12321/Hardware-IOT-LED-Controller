#include <PCF8574.h>

#define INTERRUPTMOVEBLINK 2

int buttonPin = P2;
int MovePlus = P7;
int MoveMinus = P6;
int BlinkPlus = P5;
int BlinkMinus = P4;
int Preset1 = P4;
int Preset2 = P5;
int Preset3 = P6;
int Preset4 = P7;

uint8_t Move;
uint8_t Blink;
uint8_t Button;
uint8_t MovePlusLast = LOW;
uint8_t BlinkPlusLast = LOW;
uint8_t ButtonLast = LOW;
uint8_t LEDChoice = 0;

void UpdateMoveBlink();

PCF8574::DigitalInput ReadMoveBlink;
PCF8574::DigitalInput ReadPreset;

PCF8574 MoveBlink(0x3C, INTERRUPTMOVEBLINK, UpdateMoveBlink);
PCF8574 Preset(0x38);

void setup() {
  Serial.begin(115200);

  delay(5000);

  Serial.println("Begin");

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

  Serial.println("GO");

}

void loop() {
  

}

void UpdateMoveBlink() {
  ReadMoveBlink = MoveBlink.digitalReadAll();
  Move = ReadMoveBlink.p7;
  Blink = ReadMoveBlink.p5;
  Button = ReadMoveBlink.p2;
  Serial.print("Button: ");
  Serial.println(Button);
  Serial.print("MovePlus: ");
  Serial.println(Move);
  Serial.print("MoveMinus: ");
  Serial.println(ReadMoveBlink.p6);
  Serial.print("BlinkPlus: ");
  Serial.println(Blink);
  Serial.print("BlinkMinus: ");
  Serial.println(ReadMoveBlink.p4);
  if ((MovePlusLast == LOW) && (Move == HIGH)) {
    if (ReadMoveBlink.p6 == LOW) {
      Serial.println("Move down");
    } else {
      Serial.println("Move up");
    }
  }
  MovePlusLast = Move;
  if ((BlinkPlusLast == LOW) && (Blink == HIGH)) {
    if (ReadMoveBlink.p4 == LOW) {
      Serial.println("Blink down");
    } else {
      Serial.println("Blink up");
    }
  }
  BlinkPlusLast = Blink;
  if ((Button == LOW) && (ButtonLast == HIGH)) {
    Serial.println("Button Pressed");
    ReadPreset = Preset.digitalReadAll();
    if ((!ReadPreset.p4) && (!ReadPreset.p5) && (!ReadPreset.p6) && (!ReadPreset.p7)) {
      Serial.println("No Preset Selected");
    }
    else {
      LEDChoice = !ReadPreset.p4 * 1 + !ReadPreset.p5 * 2 
                  + !ReadPreset.p6 * 4 + !ReadPreset.p7 * 8;
      Serial.print("LED Choice: ");
      Serial.println(LEDChoice);
    }
  } 
  ButtonLast = Button;
}
