#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*-----------------Variable-----------------*/
const int soilSensorPin = A3;
const int lightSensorPin = A0;
const int pumpRelayPin = 7;
const int dryValue = 432;
const int wetValue = 160;
const int humidityThreshold = 40;
const unsigned long soilCheckInterval = 12000;
const unsigned long pumpDuration = 800;
LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned long previousMillis = 0;
bool pumpActive = false;
unsigned long pumpStartTime = 0;


/*-----------------Start up-----------------*/


void setup() {
  pinMode(pumpRelayPin, OUTPUT);
  digitalWrite(pumpRelayPin, LOW);
  
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Greeen House :D");
  lcd.setCursor(0, 1);
  lcd.print("Emirlan <3");

  delay(3000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.setCursor(0, 1);
  lcd.print("Light: ");

}


/*-----------------Loop-----------------*/



void loop() {
  unsigned long currentMillis = millis();
  
  int soilRawValue = analogRead(soilSensorPin);
  int lightValue = analogRead(lightSensorPin);
  

  int soilHumidity = map(soilRawValue, dryValue, wetValue, 0, 100);
  soilHumidity = constrain(soilHumidity, 0, 100);
  

  lcd.setCursor(10, 0);
  lcd.print("    ");
  lcd.setCursor(10, 0);
  lcd.print(soilHumidity);
  lcd.print("%");
  
  lcd.setCursor(7, 1);
  lcd.print("     ");
  lcd.setCursor(7, 1);
  lcd.print(lightValue);
  
  if (pumpActive) {
    if (currentMillis - pumpStartTime >= pumpDuration) {
      digitalWrite(pumpRelayPin, LOW);
      pumpActive = false;
    }


  } else if (currentMillis - previousMillis >= soilCheckInterval) {
    previousMillis = currentMillis;
    
    if (soilHumidity < humidityThreshold) {
      digitalWrite(pumpRelayPin, HIGH);
      pumpActive = true;
      pumpStartTime = currentMillis;
    }
  }



  delay(1000);
}