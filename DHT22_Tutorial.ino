#include <LiquidCrystal_I2C.h>
#include <RFID.h>
#include <LiquidCrystal_I2C.h>
#include <LCD.h>
#include <Wire.h>
#include <DHT.h>
#include <SPI.h>

#define dht_apin A2
#define SS_PIN 10
#define RST_PIN 9

#define DHTPIN A2     // what digital pin we're connected to                           
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321 ; DHTTYPE DHT11   // DHT 11
                        
DHT DHT(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcdScreen(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  lcdScreen.begin(20,4);

  DHT.begin();
}

void loop() {
  delay(1500); // Reading temperature or humidity takes about 250 milliseconds! Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  lcdScreen.clear();
  lcdScreen.setCursor(0,0);
  lcdScreen.print("EugeneSense 2.0");
  
  float h = DHT.readHumidity();
  // Read temperature as Celsius (the default)
  float t = DHT.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = DHT.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = DHT.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = DHT.computeHeatIndex(t, h, false);

  lcdScreen.setCursor(0,2);
  lcdScreen.print("Temperature: ");
  Serial.print("\n\nTemperature: ");
  Serial.print(t);
  lcdScreen.print(t);
  Serial.print(" || ");
  Serial.print(hic);
  Serial.print(" C");
  lcdScreen.setCursor(0,3);
  lcdScreen.print("Humidity: ");
  Serial.print("\nHumidity: ");
  lcdScreen.print(h);
  Serial.print(h);
  Serial.print(" %");
  lcdScreen.print(" %");
}
