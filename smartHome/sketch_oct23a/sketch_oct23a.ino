                                                                                                                        #include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#define lampu1 D1
#define lampu2 D2
#define lampu3 D3
#define lampu4 D4
#define lampu5 D5
#define lampu6 D6


const char* ssid = "BS4C3";
const char* pass = "BSC312345";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Sedang menghubungkan ");
  }
  pinMode(lampu1,OUTPUT);
  pinMode(lampu2,OUTPUT);
  pinMode(lampu3,OUTPUT);
  pinMode(lampu4,OUTPUT);
  pinMode(lampu5,OUTPUT);
  pinMode(lampu6,OUTPUT);
  Serial.println("Sudah terhubung wifi");

}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    WiFiClient wifi;
    HTTPClient http;
    DynamicJsonDocument data(400);
    String server = "http://192.168.1.26:8000/api/status";
    http.begin(wifi,server);
    int httpResponseCode = http.GET();
    if(httpResponseCode > 0){
      String response = http.getString();
      DeserializationError error =  deserializeJson(data,response);
      JsonArray arr = data.as<JsonArray>();
      for(JsonObject obj : arr){
        String objek = obj["objek"];
        int stats = obj["status"];
        int pin = obj["pin"];
        Serial.print("objek : ");
        Serial.println(objek);
        Serial.print("status : ");
        Serial.println(stats);
        digitalWrite(pin,stats ? HIGH : LOW);
        delay(100);
      }
    } else{
       String response = http.getString();
       Serial.print("error");
       Serial.println(response);
    }
    http.end();
  }else{
    delay(1000);
    Serial.println("gagal terkoneksi sedang menghubungkan ulang");
    WiFi.reconnect();
  }
  delay(2000);
}
