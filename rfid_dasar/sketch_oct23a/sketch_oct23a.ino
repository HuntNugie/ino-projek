//untuk library dari mfrc522
#include <MFRC522.h>
//untuk komunikasi
#include <SPI.h>
#define SSPIN D1
#define RSTPIN D3

//instance class 
MFRC522 mfrc522(SSPIN,RSTPIN);


void setup(){

   Serial.begin(115200);
//    untuk inisialisasi dan membuka komunikasi
   SPI.begin();
   mfrc522.PCD_Init();
   Serial.println("rfid siap maniez");
   
}

void loop(){
//  cek jika tidak ada kartu dan tidak ada uid di kartu jangan tampilkan apa apa
  if(!mfrc522.PICC_IsNewCardPresent())return;
 if(!mfrc522.PICC_ReadCardSerial())return;

  String uid = "";
 
  Serial.print("UID : ");
//  karna uid dalam rfid nya tuh mengandung byte nya berpisah pisah maka untuk mendapatkan seluruh uid nya kita harus mengugnakan looping
 for(byte i = 0;i<mfrc522.uid.size;i++){
//  untuk mengambil per satu pecahan dari uidByte nya dan di ubah menjadi hexadesimal
  Serial.print(mfrc522.uid.uidByte[i], HEX);
  String temp = String(mfrc522.uid.uidByte[i], HEX);
  temp.toUpperCase();
  uid += temp;
  Serial.print(" ");
 
 }
 Serial.println();

  Serial.print(" hasil akhir:");
  Serial.println(uid);
//  untuk menutup komunikasi rfidnya
 mfrc522.PICC_HaltA();
 
}
