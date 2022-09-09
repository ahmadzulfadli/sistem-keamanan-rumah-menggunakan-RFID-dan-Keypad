#include <SPI.h>
#include <MFRC522.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns

int pinrelay = A1;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2};  //connect to the column pinouts of the keypad

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
String StringVal;
String Pin;
int val, pinkey = 9989;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Selamat Datang");
  lcd.setCursor(1,1);
  lcd.print("Id Card Or Pin");
  
  pinMode (pinrelay, OUTPUT);
  digitalWrite(pinrelay, HIGH);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Selamat Datang");
  Serial.println();
}

void loop() {
    
  //====================================================
  char Key = customKeypad.getKey();

  if (Key) {
    if (Key >= '0' && Key <= '9') {
      StringVal += Key;
      Pin +="*";
      Serial.println(StringVal);
      lcd.clear();
      lcd.setCursor(0, 0); 
      lcd.print("PIN : ");
      lcd.setCursor(7, 0); 
      lcd.print(Pin);
    }
    else if (Key == '#') {
      if (StringVal.length() > 0) {
        val = StringVal.toInt();
        Serial.println(val);
        if (val == pinkey){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Teridentifikasi");
          lcd.setCursor(0,1);
          lcd.print("Pintu Terbuka");
          Serial.println("Pintu Terbuka");
          digitalWrite(pinrelay, LOW);
          delay(5000);
          digitalWrite (pinrelay, HIGH);
          Pin = "";
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("Selamat Datang");
          lcd.setCursor(1,1);
          lcd.print("Id Card Or Pin");
        } else {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Tidak");
          lcd.setCursor(0,1);
          lcd.print("Teridentifikasi");
          digitalWrite(pinrelay, HIGH);
          Serial.println("Pin Salah");
          delay(5000);
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("Selamat Datang");
          lcd.setCursor(1,1);
          lcd.print("Id Card Or Pin");
        }
        StringVal = "";
      }
    }
    else if (Key == '*') {
      StringVal = "";
      Pin = "";
      lcd.clear();
      lcd.setCursor(0, 0); 
      lcd.print("PIN : ");
      lcd.setCursor(7, 0); 
      lcd.print(StringVal);
    }
  }else{
  // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
      return;
    }
    //Show UID on serial monitor
    Serial.print("UID tag :");
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
       Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
       Serial.print(mfrc522.uid.uidByte[i], HEX);
       content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
       content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == "04 58 55 4A 1F 6A 80" or content.substring(1) == "04 4F 14 CA 9C 5C 80" or content.substring(1) == "04 3F 12 52 0D 66 80" or content.substring(1) == "04 84 75 82 7D 65 80" or content.substring(1) == "04 4E 34 DA 48 63 80") //change here the UID of the card/cards that you want to give access
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Teridentifikasi");
      lcd.setCursor(0,1);
      lcd.print("Pintu Terbuka");
      Serial.println("Pintu Terbuka");
      digitalWrite (pinrelay, LOW);
      delay(5000);
      digitalWrite (pinrelay, HIGH);
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Selamat Datang");
      lcd.setCursor(1,1);
      lcd.print("Id Card Or Pin");
      
    }
   
   else   {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Tidak");
      lcd.setCursor(0,1);
      lcd.print("Teridentifikasi");
      Serial.println("Pintu Tertutup");
      digitalWrite (pinrelay, HIGH);
      delay(3000);
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Selamat Datang");
      lcd.setCursor(1,1);
      lcd.print("Id Card Or Pin");
    }
  }
 //=======================================================
  
}
