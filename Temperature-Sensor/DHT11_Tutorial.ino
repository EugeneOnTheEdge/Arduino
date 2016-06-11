#include <RFID.h>
#include <LiquidCrystal_I2C.h>
#include <LCD.h>
#include <Wire.h>
#include <dht.h>
#include <SPI.h>

#define dht_apin A2
#define SS_PIN 10
#define RST_PIN 9

dht DHT;
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcdScreen(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
int buzzer = 5;
int redLED = 6;
int greenLED = 7;

RFID rfid(SS_PIN, RST_PIN);
int serNum[5];
int cards[][5] = { {22,166,21,59,158} }; //accepted cards
bool access = false;

int getTemp = DHT.temperature;
int getHumid = DHT.humidity;
bool once = true;
int lowTemp = 99;
int lowHumid = 99;

void initDevices() {
  Serial.begin(9600); // Make sure your serial is running on 9600baud
  SPI.begin();
  rfid.init();  
  lcdScreen.backlight();
  pinMode(redLED,OUTPUT);
  pinMode(greenLED,OUTPUT);
  pinMode(buzzer,OUTPUT);
}

void setup(){ 
  initDevices(); // init all the required devices and sensors
  
  Serial.print("-----------EugeneSense Temperature and Humidity Sensor-----------\n\nPowered by: Arduino UNO R3 + DHT11 Humiture Sensor\n\n");
  
  lcdScreen.begin(16, 2);
  delay(250);
  
  lcdScreen.clear();
  lcdScreen.print("EugeneSense");
  lcdScreen.setCursor(0, 1);
  lcdScreen.print("T&H Sensor");
  digitalWrite(greenLED,HIGH);
  digitalWrite(redLED,HIGH);
  delay(2500);
  digitalWrite(greenLED,LOW);
  digitalWrite(redLED,LOW);
  lcdScreen.clear();
  lcdScreen.print("Powered by: ");
  lcdScreen.setCursor(0, 1);
  lcdScreen.print("Arduino + DHT22");
  delay(2000);
  lcdScreen.clear();
  getTemp = DHT.temperature;
  getHumid = DHT.humidity;

  lcdScreen.print("Getting T&H data");
  lcdScreen.setCursor(0,1);
  int dot = 0;
          
  for ( dot = 0 ; dot <= 20 ; dot++ )
     {
        lcdScreen.print(".");
        delay(100);
     } 
}

void loop() {
    getTemp = DHT.temperature;
    getHumid = DHT.humidity;

    //------CODE FOR LOWEST HUMIDITY AND TEMP-----------
    
    if ( getTemp != 0 && getHumid != 0 )
    {
      if ( getTemp < lowTemp )
      {
        lowTemp = getTemp;
      }

      if ( getHumid < lowHumid )
      {
        lowHumid = getHumid;
      }
    }   
    //--------------------------------------------------
    lcdScreen.clear();
    DHT.read11(dht_apin);    
    lcdScreen.print("Temp: ");
    lcdScreen.print(getTemp);
    lcdScreen.print("C");
    lcdScreen.setCursor(0, 1);
    lcdScreen.print("Hum.: ");
    lcdScreen.print(getHumid); 
    lcdScreen.print("%");

    Serial.print("\n\nTemp: ");
    Serial.print(getTemp);
    Serial.print("C | Lowest: ");
    Serial.print(lowTemp);
    Serial.print("C");
    Serial.println("\nHum.: ");
    Serial.print(getHumid); 
    Serial.print("% | Lowest: ");
    Serial.print(lowHumid); 
    Serial.print("%");
    
    delay(2000);//Wait 2.0 seconds before accessing sensor again.
}
