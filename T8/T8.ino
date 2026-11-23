#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>


unsigned long time_since = 0;
unsigned long duration = 4000;

String readID = "";

MFRC522 rfid(10, 9);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  lcd.setCursor(0, 0);
  lcd.print("SCAN CARD:");
}

void confirm() {
  lcd.setCursor(0, 1);
  lcd.print("                "); 
  lcd.setCursor(0, 1);
  lcd.print("ACCESS GRANTED");
  time_since = millis();
}

void deny() {
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("ACCESS DENIED");
  time_since = millis();
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

  Serial.print("UID: ");
  Serial.println(readID);

  if (readID == "B36D442A") {
    confirm();
  }
  else {
    deny();
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}


//----------------------------------------
void loop() {
  if (millis() - time_since >= duration) {
    lcd.setCursor(0,1);
    lcd.print("                "); 
  }

  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  readUid();

}
