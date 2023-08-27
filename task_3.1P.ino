#include <WiFiNINA.h>
#include <DHT.h>
#include <ThingSpeak.h>

#define DHTPIN 2         //THE PIN WHERE THE DIGITAL PIN OF THE DHT11 TEMPERATURE AND HUMIDITY SENSOR IS CONNECTED
#define DHTTYPE DHT11    // THERE ARE 2 TYPES OF DHT SENSOR, I HAVE USED DHT11 HERE

char ssid[] = "Pardeep_4g";     // HERE IS THE PASSWORD NAME / SSID
char pass[] = "25141810"; // HERE GOES THE PASSWORD OF THE WIFI
const char* server = "api.thingspeak.com"; //DEFINING A CONSTANT CHARACTER POINTER TO THE SERVER ADDRESS FOR MAKING HTTP REQUESTS

 WiFiClient client; // INITIALIZING THE OBJECT "client" FOR HANDLING THE WIFI CONNECTIONS

DHT dht(DHTPIN, DHTTYPE); //INITIALIZING THE DHT SENSOR
unsigned long previousMillis = 0; // USING THE UNSIGNED LONG DATATYPE AS IT STORES ONLY NON NEGATIVE WHOLE NUMBERS
const long interval = 30000;  // UPLOADING THE DATA EVERY 30 SECONDS AS DIRECTED TO US

void setup() {
  Serial.begin(9600);
  delay(100);

  // CONNECTING TO WIFI
  WiFi.begin(ssid, pass);
  // CREATING A LOOP STATEMENT THAT LOOPS UNTIL THE WIFI IS CONNECTED...(WL_CONNECTED IS A PREDEFINED WIFI STATUS CODE)
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  ThingSpeak.begin(client); // INITIALIZING THINGSPEAK
  dht.begin();              // INITIALIZING DHT11 SENSOR
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // READING TEMPERATURE AND HUMIDITY FROM THE SENSOR
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // PRINTING THE TEMPERATURE AND HUMIDITY TO THE SERIAL MONITOR
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    // SENDING THE DATA TO THINGSPEAK
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);
    
    int httpCode = ThingSpeak.writeFields(2252522, "FBYOXAY4NYH1ZP4J");

    if (httpCode == 200) {
      Serial.println("Data sent to ThingSpeak successfully!");
    } else {
      Serial.println("Error sending data to ThingSpeak. HTTP error code: " + String(httpCode));
    }
  }
  
  delay(1000); 
}
