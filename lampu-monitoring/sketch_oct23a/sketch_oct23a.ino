#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#define lampu D4
const char* ssid = "itel S25";
const char* pass = "12345678";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Sedang menghubungkan ");
  }
  pinMode(lampu,OUTPUT);
  Serial.println("Sudah terhubung wifi");

}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    WiFiClient wifi;
    HTTPClient http;
    DynamicJsonDocument data(200);
    String server = "http://10.138.156.81:3000/api/lampu/all";
    http.begin(wifi,server);
    int httpResponseCode = http.GET();
    if(httpResponseCode > 0){
      String response = http.getString();
      DeserializationError error =  deserializeJson(data,response);
      String status_lampu = data["status_lampu"];
      if(status_lampu == "nyala"){
        digitalWrite(lampu,HIGH);
        Serial.println("lampu sedang menyala");
        delay(1000);
      }else{
        digitalWrite(lampu,LOW);
        Serial.println("lampu sedang mati");
        delay(1000);
      }
    } else{
       String response = http.getString();
       Serial.print("error");
       Serial.println(response);
    }
  }else{
    delay(1000);
    Serial.println("gagal terkoneksi sedang menghubungkan ulang");
    WiFi.reconnect();
  }
  delay(2000);
}
