
#include <SPI.h>
#include <EEPROM.h>
#include <MFRC522.h>

MFRC522 rfid(10, 9);

int pinRED = 3;
int pinGREEN = 2;

String readID = "";
char authorizedID[21] = "";
bool no_master_card_yet = false;

// LED timer variables
unsigned long greenLedTime = 0;
bool greenLedActive = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();

  pinMode(pinGREEN, OUTPUT);
  pinMode(pinRED, OUTPUT);

  rfid.PCD_Init();

  EEPROM.get(10, authorizedID);

  if (authorizedID[0] == '\0' || authorizedID[0] == (char)0xFF) {
    Serial.println("Scan the Master Card....");
    no_master_card_yet = true;
  }
  else {
    Serial.println("Scan a card...");
    no_master_card_yet = false;
  }
}

void confirm() {
  digitalWrite(pinRED, LOW);
  digitalWrite(pinGREEN, HIGH);

  greenLedTime = millis();
  greenLedActive = true;
}

void deny() {
  digitalWrite(pinRED, HIGH);
}

void readUid() {
  readID = "";

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      readID += "0";
    }

    readID += String(rfid.uid.uidByte[i], HEX);
  }

  readID.toUpperCase();

  if (readID == authorizedID && no_master_card_yet == false) {
    confirm();
  }
  else if (no_master_card_yet == true) {
    confirm();
    readID.toCharArray(authorizedID, sizeof(authorizedID));
    EEPROM.put(10, authorizedID);
    no_master_card_yet = false;
  }
  else {
    deny();
  }

  Serial.print("UID: ");
  Serial.println(readID);

  rfid.PICC_HaltA();
}

void loop() {

  // Non-blocking LED timer
  if (greenLedActive && millis() - greenLedTime >= 1000) {
    digitalWrite(pinGREEN, LOW);
    greenLedActive = false;
  }

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  readUid();
}
