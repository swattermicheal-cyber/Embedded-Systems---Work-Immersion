
int GREEN_PIN = 2;
int RED_PIN = 3;


void showGranted() {
  digitalWrite(GREEN_PIN, HIGH);
  delay(1000);
  digitalWrite(GREEN_PIN, LOW);

}

void showDenied() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(RED_PIN, HIGH);
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
  showGranted();
  showDenied();
}

