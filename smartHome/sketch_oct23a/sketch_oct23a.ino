#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define lampu1 D1
#define lampu2 D2
#define lampu3 D3
#define lampu4 D4
#define lampu5 D5
#define lampu6 D6

#define buzzer D7
#define DHTPIN D8

const char *ssid = "ARIOCCI";
const char *pass = "davaric1";

DHT dht(DHTPIN, DHT22);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Sedang menghubungkan ");
  }
  dht.begin();
  pinMode(buzzer, OUTPUT);
  pinMode(lampu1, OUTPUT);
  pinMode(lampu2, OUTPUT);
  pinMode(lampu3, OUTPUT);
  pinMode(lampu4, OUTPUT);
  pinMode(lampu5, OUTPUT);
  pinMode(lampu6, OUTPUT);
  Serial.println("Sudah terhubung wifi");
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClient wifi;
    HTTPClient http;
    DynamicJsonDocument data(400);
    String server = "http://192.168.100.204:8000";
    String smartHome = server + "/api/status";
    http.begin(wifi, smartHome);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0)
    {
      String response = http.getString();
      DeserializationError error = deserializeJson(data, response);
      JsonArray arr = data.as<JsonArray>();
      for (JsonObject obj : arr)
      {
        String objek = obj["objek"];
        int stats = obj["status"];
        int pin = obj["pin"];
        Serial.print("objek : ");
        Serial.println(objek);
        Serial.print("status : ");
        Serial.println(stats);
        digitalWrite(pin, stats ? HIGH : LOW);
        delay(100);
      }
    }
    else
    {
      String response = http.getString();
      Serial.print("error");
      Serial.println(response);
    }
    http.end();

    float temp = dht.readTemperature();
    float humd = dht.readHumidity();
    String dhtUrl = server + "/api/dht22/update/" + String(temp) + "/" + String(humd);
    http.begin(wifi, dhtUrl);
    Serial.print("temperature : ");
    Serial.println(temp);
    Serial.print("humidity: ");
    Serial.println(humd);

    if (isnan(temp) || isnan(humd))
    {
      Serial.println("gagal membaca dht");
      delay(1000);
    }
    int httpResponseDht = http.GET();
    if (httpResponseDht > 0)
    {
      String response = http.getString();
      DynamicJsonDocument doc(200);
      deserializeJson(doc, response);
      int resTemp = doc["temperature"];
      int resHumd = doc["humidity"];
      Serial.print("response");
      Serial.println(response);
    }
    else
    {
      String response = http.getString();
      Serial.print("response");
      Serial.println(response);
    }
    http.end();

    String maxUrl = server + "/api/dht22/max";
    http.begin(wifi, maxUrl);
    int httpResponseMax = http.GET();
    if (httpResponseMax > 0)
    {
      String response = http.getString();
      DynamicJsonDocument doc(200);
      deserializeJson(doc, response);
      int maxSuhu = doc["max_temp"];
      Serial.print("response");
      Serial.println(response);

      if (temp >= maxSuhu)
      {
        digitalWrite(buzzer, HIGH);
        Serial.println("panasssss sayangg");
        delay(2000);
      }
      else
      {
        digitalWrite(buzzer, LOW);
        Serial.println("dinginnnn sayangg");
        delay(2000);
      }
    }
    http.end();
  }
  else
  {
    delay(1000);
    Serial.println("gagal terkoneksi sedang menghubungkan ulang");
    WiFi.reconnect();
  }
  delay(2000);
}