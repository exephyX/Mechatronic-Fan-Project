/*Temperature Humidity Fan Sketch - Tyler Sexton, Nicholas Shutt*/
//version 0.3//
#include <DHT.h>                  // Adafruit library
#include <LiquidCrystal.h>
////////////////////////////////////////////////////////////////////////////////////////////
#define DHTPIN A1                 // DHT22 sensor pin
#define DHTTYPE DHT22             // DHT22 sensor type
#define RELAYPIN A0               // Relay Module pin

/////////Liquid Crystal/////////////////////////////////////////////////////////////////////
const int rs = 2,
          en = 3,
          d4 = 6,
          d5 = 7,
          d6 = 8,
          d7 = 9;
////////////////////////////////////////////////////////////////////////////////////////////
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
////////////////////Functions///////////////////////////////////////////////////////////////
void SetLCD(){
  lcd.clear();
  lcd.setCursor(0, 0);
}
void PrintResult(float temperature, float humidity){
  SetLCD();
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  delay(4000);
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  delay(4000);
}
void FanRamp(float a){
      float b = 0;
      a = 30*a + 128;
      for(b;b<255;b+=(a/255)){
        analogWrite(RELAYPIN,b);
        delay(100);

        SetLCD();
        lcd.print(b);
      }
}
// void Debug(String temperature, String humidity) {
//   Serial.print("Temperature: ");
//   Serial.print(temperature);
//   Serial.println("°C");

//   Serial.print("Humidity: ");
//   Serial.print(humidity);
//   Serial.println("%");
// }
void Validity(int temperature, int humidity) {
  if (isnan(humidity) || isnan(temperature)) {       // Validity check
    //Serial.println("Error: Invalid reading from DHT22 sensor!");
    SetLCD();
    lcd.print("DHT22 Error!");
    delay(5000);
    SetLCD();
    lcd.print("Returning 2 loop");
    return;
  }
}
// Fan on for 21 sec each
void Mode1(float temperature, float humidity){
// Demo
  delay(1500);
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(); 
  PrintResult(temperature, humidity);
  SetLCD();
  lcd.print("Mode 1");
  int i = 0;
  int f = 0;
  for(i; i < 3; i++) {
    lcd.blink();
    lcd.print(". ");
    delay(1500);
  }
  lcd.noBlink();
  if (humidity >= 40) {                // Fan on if humidity is above 40%
    SetLCD();
    lcd.print("Humi. > 40 . . ."); 
    delay(1500);
    analogWrite(RELAYPIN, 255);
    SetLCD();
    for(i=0; i < 13; i++) {
    lcd.setCursor(i,0);
    lcd.print(".");
    delay(1000);
    }
    humidity = dht.readHumidity();
    temperature = dht.readTemperature(); 
    PrintResult(temperature, humidity);
    SetLCD();
    analogWrite(RELAYPIN, 0); 
    lcd.print("Fan Off"); 
    delay(3000);
        
  } else {                             
    SetLCD();
    lcd.print("Nominal Humidity!");
    delay(1000);
    lcd.println("     Readings:");
    delay(2000);
    PrintResult(temperature, humidity);
  }

//-  Alt. Temperature In-line Code -
  if (temperature > 25) {                // Fan on if humidity is above 40%
    SetLCD();
    lcd.print("Temp. > 25 . . ."); 
    delay(1500);
    analogWrite(RELAYPIN, 255);
    SetLCD();
    for(i=0; i < 13; i++) {
    lcd.setCursor(i,0);
    lcd.print(".");
    delay(1000);
    }
    humidity = dht.readHumidity();
    temperature = dht.readTemperature(); 
    PrintResult(temperature, humidity);
    SetLCD();
    analogWrite(RELAYPIN, 0); 
    lcd.print("Fan Off"); 
    delay(3000);
  }   
   else {                             
    SetLCD();
    lcd.print("Nominal Temp!");
    delay(1000);
    lcd.println("     Readings:");
    delay(2000);
    humidity = dht.readHumidity();
    temperature = dht.readTemperature(); 
    PrintResult(temperature, humidity);
  }
}
void Mode2(float temperature, float humidity){
// Fan Calibration
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(); 
  PrintResult(temperature, humidity);
  SetLCD();
  lcd.print("Mode 2");
  int i = 0;
  int f = 0;
  for(i; i < 3; i++) {
    lcd.blink();
    lcd.print(". ");
    delay(1500);
  }
  lcd.noBlink();

  float stemp = 25, shumi = 40;
  float a, b;
  int step;

  if(stemp < temperature || shumi < humidity) {
    SetLCD();
    String S = "Not Nominal!";
    lcd.blink();
    for(int i = 0; i < S.length(); i++){
      lcd.print(S.charAt(i));
      delay(500);      
    }
    lcd.noBlink();
    if(stemp < temperature) {
      a = temperature - stemp;
      humidity = dht.readHumidity();
      temperature = dht.readTemperature(); 
      PrintResult(temperature, humidity);
      SetLCD();
      lcd.print("Temp Diff: " + (String)a);
      delay(3000);
      FanRamp(a);
      delay(3000);
      analogWrite(RELAYPIN, 0);
      SetLCD();
      humidity = dht.readHumidity();
      temperature = dht.readTemperature(); 
      PrintResult(temperature, humidity);
      b = temperature - stemp;
      SetLCD();
      lcd.print("Temp Diff: " + (String)b);
      delay(3000);
      step = int(a) - int(b);
      SetLCD();
      lcd.print("T is "+ (String)step + " lower!");
      delay(5000);
    }
    if(shumi<humidity){
      SetLCD();
      lcd.print("Humi. Func N/A!");
      delay(3000);
    }    
  }
  else{
    SetLCD();
    String S = "Nominal!";
    lcd.blink();
    for(int i = 0; i < S.length(); i++){
      lcd.print(S.charAt(i));
      delay(500);      
    }
    lcd.noBlink();
  }

}
void Mode3(float temperature, float humidity){
// Fan Countdown
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(); 
  PrintResult(temperature, humidity);
  SetLCD();
  lcd.print("Mode 3");
  int i = 0;
  int f = 0;
  for(i; i < 3; i++) {
    lcd.blink();
    lcd.print(". ");
    delay(1500);
  }
  lcd.noBlink();
  SetLCD();
  lcd.print("Fan On!");
  analogWrite(RELAYPIN, 255);
  for(int i=0; i < 7; i++) {
    lcd.setCursor(i,1);
    lcd.print(".");
    delay(1000);
    }
  for(int i = 6; i>=0;i--){
    lcd.setCursor(i,1);
    lcd.print(" ");
    delay(1000);
  }
  SetLCD();
  lcd.print("Fan Off!");
  analogWrite(RELAYPIN, 0);
  SetLCD();
}
////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(RELAYPIN, OUTPUT);
  analogWrite(RELAYPIN, 0);    // Relay off
  Serial.begin(9600);
  delay(50);
  dht.begin();
  lcd.begin(16,2);
}
////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  SetLCD();
  lcd.print("Welcome!");
  delay(300);
  lcd.setCursor(0,1);
  lcd.print("This is a demo!");
  delay(2000);
  SetLCD();
  lcd.blink();
  for(int i=0; i < 7; i++) {
    lcd.setCursor(i,1);
    lcd.print(".");
    delay(1000);
    }
  lcd.noBlink();
  float humidity = dht.readHumidity();               // Humidity value from sensor
  float temperature = dht.readTemperature();         // Temperature value from sensor
////////////////////////////////////////////////////////////////////////////////////////////
Validity(temperature, humidity);
////////////////////////////////////////////////////////////////////////////////////////////
Mode1(temperature, humidity); 
Mode2(temperature, humidity);
Mode3(temperature, humidity);
////////////////////////////////////////////////////////////////////////////////////////////
  delay(1000);  // Second delay before next sensor read
}
////////////////////////////////////////////////////////////////////////////////////////////
/*To-Do / Improvements(?) Excluding Testing...
1) Implement calls to functions over inline code (Unsure if arduino boards can utilize function calls).
2) Implement a sort of flagging system to call for events in code (e.g. 30 seconds after passing flagged temperature value lower/raise flag speed gradually)
    Measures temperature change and sees if benificial enough after x cycles to continue
3) LCD displays active mode and readings
      4) Not inline but not interrupt
//NOTES:
Circuit is not 100% complete. The relay needs to be connected to a separate power source or something needs to be done to guarantee the internal switch acts as desired.
If a second power source is required; I have 2 other boards I can bring over in our next lab.
Best, T.S
*/