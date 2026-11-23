
#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>


// Timer
unsigned long time_since = 0;
unsigned long duration = 4000;
bool messageActive = false;


//Buzzer
#define BUZZER A2

// RFID
MFRC522 rfid(10, 9);

// LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// EEPROM storage
const int MASTER_FLAG_ADDR = 0;
const int MASTER_LENGTH_ADDR = 1;
const int MASTER_UID_ADDR = 2;

const byte MASTER_FLAG = 0xA5;
const byte MAX_UID_SIZE = 10;

byte masterUID[MAX_UID_SIZE];
byte masterUIDSize = 0;

bool masterRegistered = false;


//Play a sound
void Buzzerbeep(int duration, int freq) {
  tone(BUZZER, freq, duration);
}

void successTune() {
  tone(BUZZER, 523, 150); // C5
  delay(180);

  tone(BUZZER, 659, 150); // E5
  delay(180);

  tone(BUZZER, 784, 250); // G5
  delay(280);

  noTone(BUZZER);
}

void deniedTune() {
  tone(BUZZER, 300, 250);
  delay(300);

  tone(BUZZER, 220, 400);
  delay(450);

  noTone(BUZZER);
}

// Display a temporary message
void showMessage(String top, String bottom) {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(top);

  lcd.setCursor(0, 1);
  lcd.print(bottom);

  time_since = millis();
  messageActive = true;
}


// Load master card from EEPROM
void loadMaster() {
  if (EEPROM.read(MASTER_FLAG_ADDR) == MASTER_FLAG) {

    byte storedLength = EEPROM.read(MASTER_LENGTH_ADDR);

    if (storedLength >= 1 && storedLength <= MAX_UID_SIZE) {

      masterUIDSize = storedLength;

      for (byte i = 0; i < masterUIDSize; i++) {
        masterUID[i] = EEPROM.read(MASTER_UID_ADDR + i);
      }

      masterRegistered = true;

      Serial.println("Master card loaded!");

    } else {
      masterRegistered = false;
    }
  }
  else {
    masterRegistered = false;
  }
}


// Save first scanned card as master
void saveMaster() {

  masterUIDSize = rfid.uid.size;

  for (byte i = 0; i < masterUIDSize; i++) {
    masterUID[i] = rfid.uid.uidByte[i];

    EEPROM.update(MASTER_UID_ADDR + i, masterUID[i]);
  }

  EEPROM.update(MASTER_LENGTH_ADDR, masterUIDSize);
  EEPROM.update(MASTER_FLAG_ADDR, MASTER_FLAG);

  masterRegistered = true;

  Serial.println("MASTER CARD REGISTERED!");

  showMessage("MASTER SAVED", "Registration OK");
}


// Check if scanned card is master
bool isMaster() {

  if (rfid.uid.size != masterUIDSize) {
    return false;
  }

  for (byte i = 0; i < masterUIDSize; i++) {
    if (rfid.uid.uidByte[i] != masterUID[i]) {
      return false;
    }
  }

  return true;
}


// Read card UID
void readUid() {

  Serial.print("UID: ");

  for (byte i = 0; i < rfid.uid.size; i++) {

    if (rfid.uid.uidByte[i] < 0x10) {
      Serial.print("0");
    }

    Serial.print(rfid.uid.uidByte[i], HEX);
  }

  Serial.println();


  // First card becomes master
  if (!masterRegistered) {
    saveMaster();
  }

  // Check against saved master
  else if (isMaster()) {
    showMessage("ACCESS GRANTED:", "Proceed");
    successTune();
  }

  else {
    showMessage("ACCESS DENIED:", "Dont Proceed");
    deniedTune();
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}


// Setup
void setup() {
  pinMode(BUZZER, OUTPUT);

  lcd.begin(16, 2);
  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  loadMaster();

  if (masterRegistered) {
    lcd.setCursor(0, 0);
    lcd.print("SCAN CARD:");
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("REGISTER MASTER");

    lcd.setCursor(0, 1);
    lcd.print("Scan first card");
  }
}


// Main loop
void loop() {

  // Non-blocking LCD timer
  if (messageActive && millis() - time_since >= duration) {

    lcd.clear();

    if (masterRegistered) {
      lcd.setCursor(0, 0);
      lcd.print("SCAN CARD:");
    }
    else {
      lcd.setCursor(0, 0);
      lcd.print("REGISTER MASTER");

      lcd.setCursor(0, 1);
      lcd.print("Scan first card");
    }

    messageActive = false;
  }


  // Check RFID
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  readUid();
}
