const byte BUZ = 5;
const int notes[] = {262, 294, 330, 349, 392, 440, 494, 523};   // C4 to C5
int GREEN_PIN = 2;
int RED_PIN = 3;


void showGranted() {
  digitalWrite(GREEN_PIN, HIGH);
  beepGranted();
  delay(1000);
  digitalWrite(GREEN_PIN, LOW);

}

void showDenied() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(RED_PIN, HIGH);
    beepDenied();
    delay(350);
    digitalWrite(RED_PIN, LOW);
    delay(350);
  }
}


void setup() {
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
}

void loop() {
  // for (byte i = 0; i < 8; i++) {
  //   tone(BUZ, notes[i], 200);
  //   delay(250);
  // }
  //delete this later cuz its not gonna get used anymore
  showGranted();
  showDenied();
}

void beepGranted() {
  tone(BUZ, 1000, 150);
  delay(100);
}

void beepDenied() {
  tone (BUZ, 200, 600);
  delay(100);
}