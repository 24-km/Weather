#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1 
#define CLK_PIN   13  
#define DATA_PIN  11  
#define CS_PIN    10  

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const char *ssid = "Vivo v29";
const char *password = "123the789";

const char *apiKey = "lat=44.34&lon=10.99&appid";
const char *city = "Chennai";

void setup() {
  P.begin();
  connectToWiFi();
}

void loop() {
  getWeather();
  delay(300000); 
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void getWeather() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?q=";
    url += city;
    url += "&appid=";
    url += apiKey;

    http.begin(url);

    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      JSONVar weatherData = JSON.parse(payload);
      double temperature = weatherData["main"]["temp"];
      double humidity = weatherData["main"]["humidity"];
      String weatherDesc = weatherData["weather"][0]["description"];

      String displayText = "Temp: " + String(temperature) + "C Humidity: " + String(humidity) + "% " + weatherDesc;
      P.displayText(displayText, PA_CENTER, 1000, 1000, PA_SCROLL_LEFT);
    }

    http.end();
  }
}