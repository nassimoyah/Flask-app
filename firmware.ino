#define RX_PIN 14
#define TX_PIN 13
String receivedMessage ;
/////////////////////////////////////////////////////////////////
#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>  // Use the correct library name
/////////////////////////////////////////////////////////////////////////////////
#define DHTPIN 26     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
float t ;
///////////////////////////////////////
const char* ssid = "DJAWEB";
const char* password = "89072323";
char jsonOutput [128];

void setup() {
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  temp() ;
  Serial.println(F("DHTxx test!"));
  dht.begin();
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (Serial2.available()){
    receivedMessage = Serial2.readStringUntil('/');
    Serial.print("Received message: ");
    Serial.println(receivedMessage); }
  

    
    delay(10);
    
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient client;
    client.begin("http://192.168.1.2:5555/api/sensors/6614153d063915516c4183fb");  
    client.addHeader("Content-Type", "application/json");
    temp() ;
    JSONVar postData;
    postData["id"] = "12";
    postData["lat"] = 100;
    postData["long"] = receivedMessage ;
    postData["temp"] = t ; 
    postData["pulses"] = 100 ;
    
    // Convert JSON object to string
    String jsonString = JSON.stringify(postData);
    // Send POST request with JSON data
    int httpResponseCode = client.PATCH(jsonString);
    
    
    if (httpResponseCode > 0) {
      if (httpResponseCode == HTTP_CODE_OK) {
        String payload = client.getString();
        Serial.print("Response: ");
        Serial.println(payload);
      } else {
        Serial.print("HTTP request failed with error code: ");
        Serial.println(httpResponseCode);
      }
    } else {
      Serial.println("Connection failed");
    }
  } else {
    Serial.println("WiFi not connected");
  }
  delay(10000);

  
}

void temp(){
  delay(200);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
   t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
