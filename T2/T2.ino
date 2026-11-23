const byte BTN = 2, LED = 7;
bool ledOn = false;
bool lastReading = HIGH;
bool BTNPR = false;
unsigned long lastChange = 0;
unsigned long Led_Onpassed_Time = 0;
const unsigned long duration = 3000; // Time to keep LED on (3 seconds)
int Button_Pressed = 0;

void setup() {
  pinMode(BTN, INPUT_PULLUP);   // pressed = LOW
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void showOn() {
  if (BTNPR == true) {
    digitalWrite(LED, HIGH);
    
    if (millis() - Led_Onpassed_Time >= duration) {
      digitalWrite(LED, LOW);
      BTNPR = false;
    }
  }
}

void loop() {
  bool reading = digitalRead(BTN);
  if (reading != lastReading && millis() - lastChange > 30) {   // 30 ms debounce
    lastChange = millis();
    lastReading = reading;
    if (reading == LOW && BTNPR == false) {                // new press
      ledOn = !ledOn;
      Button_Pressed += 1;
      Led_Onpassed_Time = millis();
      BTNPR = true;
      Serial.print("Button Pressed: ");
      Serial.println(Button_Pressed);
    }

    else if (reading == LOW && BTNPR == true) {
      //Serial.println("NOT VALID");
    }
  }

  showOn();

}
