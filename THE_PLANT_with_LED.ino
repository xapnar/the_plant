#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
/*-----------------Variable-----------------*/
const int soilSensorPin = A3;
const int lightSensorPin = A0;
const int pumpRelayPin = 7;
const int lightRelayPin = 2;
const int neoPixelPin = 6;
const int dryValue = 432;
const int wetValue = 160;
const int numLEDs = 24;
const int humidityThreshold = 40;
const int lightThreshold = 20;
const unsigned long soilCheckInterval = 12000; // 12s
const unsigned long pumpDuration = 700; // 0.7 second


LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numLEDs, neoPixelPin, NEO_GRB + NEO_KHZ800);


unsigned long previousMillis = 0;
bool pumpActive = false;
unsigned long pumpStartTime = 0;


/*-----------------start up-----------------*/


void setup() {
  pinMode(pumpRelayPin, OUTPUT);
  digitalWrite(pumpRelayPin, LOW);
  

  pinMode(lightRelayPin, OUTPUT);
  digitalWrite(lightRelayPin, LOW);
  

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Greeen House :D");
  lcd.setCursor(0, 1);
  lcd.print("Emirlan <3");


  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.setCursor(0, 1);
  lcd.print("Light: ");
  

  pixels.begin();
  pixels.clear();
  pixels.show();
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
  
  if (lightValue < lightThreshold) {
    digitalWrite(lightRelayPin, HIGH);
    
    for (int i = 0; i < numLEDs; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    }
    pixels.show();
  } else {
    digitalWrite(lightRelayPin, LOW);
    
    pixels.clear();
    pixels.show();
  }
}