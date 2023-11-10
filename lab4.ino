#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"

const char* ssid = "NXNG0917";
const char* password = "========";
unsigned long channelID = 2339881;
const char* writeKey = "TQIOEJGNHOYFE1CA";

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;



#define DHTPIN D4
#define DHTTYPE DHT11

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  dht.begin();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("Attempting to connect");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(5000);
      }
      Serial.println("\nConnected.");
    }

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%  Temperature: ");
    Serial.print(temperature);
    Serial.println("°C ");

    // Update the ThingSpeak channel with the new sensor data
    ThingSpeak.setField(1, temperature); // Set the first field of the channel to temperature
    ThingSpeak.setField(2, humidity);    // Set the second field of the channel to humidity

    
    int x = ThingSpeak.writeFields(channelID, writeKey);
    if (x == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }
}
