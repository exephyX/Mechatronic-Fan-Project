/*Temperature Humidity Fan Sketch - Tyler Sexton, Nicholas Shutt*/
//version 0.3//
#include <DHT.h>                  // Adafruit library
////////////////////////////////////////////////////////////////////////////////////////////
#define DHTPIN A1                 // DHT22 sensor pin
#define DHTTYPE DHT22             // DHT22 sensor type
#define RELAYPIN A0               // Relay Module pin
DHT dht(DHTPIN, DHTTYPE);
////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);    // Relay off
  Serial.begin(9600);
  dht.begin();
}
////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  float humidity = dht.readHumidity();               // Humidity value from sensor
  float temperature = dht.readTemperature();         // Temperature value from sensor
  if (isnan(humidity) || isnan(temperature)) {       // Validity check
    Serial.println("Error: Invalid reading from DHT22 sensor!");
    return;
  }
////////////////////////////////////////////////////////////////////////////////////////////
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
////////////////////////////////////////////////////////////////////////////////////////////
  if (humidity >= 43) {                // Fan on if humidity is above 60%
    digitalWrite(RELAYPIN, HIGH);
    Serial.println("Humidity: Trigger Fan"); 
    delay(1000);
    digitalWrite(RELAYPIN, LOW); 
    Serial.println("Humidity: OFF");     
  } else {                             
    digitalWrite(RELAYPIN, LOW); 
    delay(4000);
    digitalWrite(RELAYPIN, HIGH);  
    delay(4000);
    digitalWrite(RELAYPIN, LOW);
  }

//-  Alt. Temperature In-line Code -
  if (temperature > 0) { 
    digitalWrite(RELAYPIN, HIGH);
    Serial.println("Temperature: Trigger Fan"); 
    delay(1000);
    digitalWrite(RELAYPIN, LOW); 
    Serial.println("Temperature: OFF"); 
         
  } else {                             
    digitalWrite(RELAYPIN, LOW); 
    delay(4000);
    digitalWrite(RELAYPIN, HIGH);  
    delay(4000);
    digitalWrite(RELAYPIN, LOW);     
  }

////////////////////////////////////////////////////////////////////////////////////////////
  delay(1000);  // Second delay before next sensor read
}
////////////////////////////////////////////////////////////////////////////////////////////
/*To-Do / Improvements(?) Excluding Testing...
1) Implement calls to functions over inline code (Unsure if arduino boards can utilize function calls).
2) Implement a sort of flagging system to call for events in code (e.g. 30 seconds after passing flagged temperature value lower/raise flag speed gradually)
3) Temperature logging after conditions met(?) Not useful for us.
//NOTES:
Circuit is not 100% complete. The relay needs to be connected to a separate power source or something needs to be done to guarantee the internal switch acts as desired.
If a second power source is required; I have 2 other boards I can bring over in our next lab.
Best, T.S
*/
