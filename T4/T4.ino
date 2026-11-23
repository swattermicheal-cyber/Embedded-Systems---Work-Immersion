#include <LiquidCrystal.h>

const byte BTN = 7;

bool lastReading = HIGH;
bool messageActive = false;

unsigned long lastChange = 0;
unsigned long ButtongTime = 0;

const unsigned long duration = 2000;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int trigPin = 9;
const int echoPin = 10;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(BTN, INPUT_PULLUP);

  lastReading = digitalRead(BTN);

  lcd.begin(16, 2);
  Serial.begin(9600);

  lcd.print("DISTANCE: ");
}

void loop() {
  bool reading = digitalRead(BTN);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distanceCm = (duration * 0.034) / 2.0;

  // Button detection with debounce
  // if (reading != lastReading && millis() - lastChange > 30) {
  //   lastChange = millis();
  //   lastReading = reading;

  //   if (reading == LOW) {
  //     Serial.println("CLICKED");

  //     lcd.setCursor(0, 1);
  //     lcd.print("ACCESS GRANTED");

  //     ButtongTime = millis();
  //     messageActive = true;
  //   }
  // }

}
