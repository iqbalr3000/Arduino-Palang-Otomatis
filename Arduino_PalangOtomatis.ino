//menggunakan library
#include<SPI.h>
#include<MFRC522.h>
#include<LiquidCrystal_I2C.h>
#include<Servo.h>

//inisialisasi variable mfrc522
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

//inisialisasi variable lcd
LiquidCrystal_I2C lcd(0x27, 16, 2);

//inisialisasi variable servo
int outServo = 4;
Servo myServo;

//inisialisasi variabel infrared
int infrared = 3;

//inisialisasi variable tambahan
int buzzer = 2;
int kunci = 0;
String uidString;
int R = 5;
int Y = 6;
int G = 7;
int saldo = 15000;
int incomingByte = 0;
int input = 0;

void setup() {
  //setup baudrate
  Serial.begin(9600);

  //initiate spi
  SPI.begin();

  //initiate mfrd522
  mfrc522.PCD_Init();

  //initiate lcd
  lcd.begin();

  //initiate servo
  myServo.attach(outServo);

  //setup mode pin input atau output
  pinMode(infrared,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(R,OUTPUT);
  pinMode(Y,OUTPUT);
  pinMode(G,OUTPUT);

  //kondisi awal lcd
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tempelkan Kartu!");

  //kondisi awal servo
  myServo.write(90);

  //kondisi awal lampu
  digitalWrite(R,LOW);
  digitalWrite(Y,LOW);
  digitalWrite(G,LOW);

  //tambahkan saldo menu
  Serial.println("Silahkan Pilih salah satu:");
  Serial.println("1.Cek Saldo");
  Serial.println("2.Tambah Saldo Rp. 5000");
  Serial.println("3.Tambah Saldo Rp. 10000");

}

void loop() {
  //baca data
  if(mfrc522.PICC_IsNewCardPresent() && kunci == 0){
    readRFID();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tag ID : ");
    lcd.setCursor(0,1);
    lcd.print(uidString);
    
    if(uidString == "8A-37-5A-1A"){
      if(saldo >= 5000){
        buzzerBerhasil();
        hijauON();
        saldo -= 5000;
        kunci = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("BERHASIL!");
        delay(500);
        lcd.setCursor(0,1);
        lcd.print("Sisa Saldo : " + String(saldo));
        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Palang dibuka");
        delay(1000);
        myServo.write(180);  
      }else{
        buzzerGagal();
        merahON();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Saldo tidak mencukupi");
        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Sisa Saldo : " + String(saldo));
        delay(1000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Tempelkan Kartu!");
      }
    }else{
      buzzerGagal();
      merahON();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("GAGAL!");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Tempelkan Kartu!");
    }
  }

  tambahkanSaldo();

  infraredON();

}

void readRFID(){
  mfrc522.PICC_ReadCardSerial();
  uidString = String(mfrc522.uid.uidByte[0], HEX) + "-" +
              String(mfrc522.uid.uidByte[1], HEX) + "-" +
              String(mfrc522.uid.uidByte[2], HEX) + "-" +
              String(mfrc522.uid.uidByte[3], HEX);
              uidString.toUpperCase();
}

void infraredON(){
  int readInfrared = digitalRead(infrared);

  if(readInfrared == 0 && kunci == 1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Palang ditutup");
    delay(1000);
    myServo.write(90);
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tempelkan Kartu!");
    kunci = 0;
  }
}

void buzzerBerhasil(){
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
  delay(100);
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
}

void buzzerGagal(){
  digitalWrite(buzzer,HIGH);
  delay(500);
  digitalWrite(buzzer,LOW);
}

void merahON(){
  digitalWrite(R,HIGH);
  digitalWrite(Y,LOW);
  digitalWrite(G,LOW);
  delay(1000);
  digitalWrite(R,LOW);
}

void kuningON(){
  digitalWrite(R,LOW);
  digitalWrite(Y,HIGH);
  digitalWrite(G,LOW);
  delay(1000);
  digitalWrite(Y,LOW);
}

void hijauON(){
  digitalWrite(R,LOW);
  digitalWrite(Y,LOW);
  digitalWrite(G,HIGH);
  delay(1000);
  digitalWrite(G,LOW);
}

void tambahkanSaldo(){
  incomingByte = Serial.read();
  input = incomingByte - 48;

  switch (input) { 
       case 1:
         Serial.println(saldo);
         break;
       case 2:
         int tambahSaldo5000 = 5000;
         saldo += tambahSaldo5000;
         Serial.println(saldo);
         break;
       case 3:
         int tambahSaldo10000 = 10000;
         saldo += tambahSaldo10000;
         Serial.println(saldo);
         break;
     }
  
}
