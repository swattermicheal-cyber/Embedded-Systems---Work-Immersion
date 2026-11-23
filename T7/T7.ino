#include <SPI.h>
#include <MFRC522.h>

MFRC522 rfid(10, 9);

int pinRED = 3;
int pinGREEN = 2;

String readID = "";

void setup() {
  Serial.begin(9600);
  SPI.begin();

  pinMode(pinGREEN, OUTPUT);
  pinMode(pinRED, OUTPUT);

  rfid.PCD_Init();
  Serial.println("Scan a card...");
}

void confirm() {
  digitalWrite(pinRED, LOW);
  digitalWrite(pinGREEN, HIGH);
  delay(2000);
  digitalWrite(pinGREEN, LOW);
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

  if (readID == "B36D442A") {
    confirm();
  }
  else {
    deny();
  }

  Serial.print("UID: ");
  Serial.println(readID);

  rfid.PICC_HaltA();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    deny();
    return;
  }

  readUid();
}