//untuk library dari mfrc522
#include <MFRC522.h>
 //untuk komunikasi
#include <SPI.h>

#include <ESP8266WiFi.h>

#include <WiFiClient.h>

#include <ArduinoJson.h>

#include <ESP8266HTTPClient.h>


#define SSPIN D1
#define RSTPIN D3

//instance class 
MFRC522 mfrc522(SSPIN, RSTPIN);

String ssid = "BS4C3";
String password = "BSC312345";

String server = "http://192.168.1.26:8000";
WiFiClient wifi;
HTTPClient http;

String getUid() {

  //  ini baru pasang apa yang di perlukan
  String uid = "";

  Serial.print("UID : ");
  //  karna uid dalam rfid nya tuh mengandung byte nya berpisah pisah maka untuk mendapatkan seluruh uid nya kita harus mengugnakan looping
  for (byte i = 0; i < mfrc522.uid.size; i++) {
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

  return uid;
}


void setup() {

  Serial.begin(115200);
  //    untuk inisialisasi dan membuka komunikasi
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("rfid siap maniez");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi terhubung");

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    //  cek jika tidak ada kartu dan tidak ada uid di kartu jangan tampilkan apa apa
    if (!mfrc522.PICC_IsNewCardPresent()) return;
    if (!mfrc522.PICC_ReadCardSerial()) return;
    String uid = getUid();
    String modeServer = server+"/mode/status-mode";
    String modeUser = "";

//  ini untuk mengambil mode 
    http.begin(wifi,modeServer);
    int httpresponseCode = http.GET();
    if(httpresponseCode > 0){
      String response = http.getString();
      DynamicJsonDocument doc(200);
      deserializeJson(doc,response);
      modeUser = doc["mode"].as<String>();
   }else{
      String response = http.getString();
      Serial.println(response);
  }
   http.end();
//  untuk mengupdate mode
    if(modeUser == "register" || modeUser == "login"){
      String updateModeServer = server+"/mode/set-status/true/"+uid;
      http.begin(wifi,updateModeServer);
      int httpResponseCodeUpdate = http.GET();
      if(httpResponseCodeUpdate > 0){
        String response = http.getString();
        Serial.println(response);
      }else{
         String response = http.getString();
        Serial.println(response);
      }
    }else{
      Serial.println("sedang tidak memilih menu");  
    }
    http.end();
  } else {
    WiFi.reconnect();
    Serial.println("sedang menyambungkan");
  }

}