/*
  This is my physical  Arduino setup:
  
  Arduino board: Mega2560
  RFID RFID-RC522/522A connections:
    SDA: 53
    SCK: 52
    MOSI: 51
    MISO: 50
    IRQ: [leave unconnected]
    GND: GND
    RST: 5
    3.3V: 3.3V
  
  Stepper motor 28BYJ-48 *board* connections:
    +: 5V
    -: GND
    IN1: 10
    IN2: 11
    IN3: 12
    IN4: 13
  
  Arduino > miscellaneous stuffs connections:
    Red LED: 8
    Yellow LED: 6
    Green LED: 7
    
  Don't forget to change the UID of the cards which the Arduino accepts. Scroll down and you'll see a huge line break, and you'll see int cards[][5].
*/

#include <Tone.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RFID.h>
#define SS_PIN 53
#define RST_PIN 5
//----------------STEPPER MOTOR---------------------------------
#define IN1  10
#define IN2  11
#define IN3  12
#define IN4  13
//-----------------------------

void SetDirection(){
  if(Direction==1){ Steps++;}
  if(Direction==0){ Steps--; }
  if(Steps>7){Steps=0;}
  if(Steps<0){Steps=7;}
}

void stepper(int xw){
  for (int x=0;x<xw;x++){
  switch(Steps){
   case 0:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   case 1:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, HIGH);
   break; 
   case 2:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 3:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 4:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 5:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
     case 6:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 7:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   default:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
  }
  SetDirection();
  }
} 

void turnMotor(int angle)
{
  steps_left = angle * 11.377777778;
  
  while(steps_left>0)
  {
    currentMillis = micros();
    digitalWrite(yellow, HIGH);
    if(currentMillis-last_time>=1000)
    {
      stepper(1); 
      time=time+micros()-last_time;
      last_time=micros();
      steps_left--;
    }
  }
  digitalWrite(yellow,LOW);
  
  // Direction=!Direction; // changes direction everytime
  // steps_left=1024; // 90deg
}
//----------------------------------------------------------------
LiquidCrystal_I2C lcdScreen(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Tone buzzer;

int serNum[5];
int yellow = 6;
int green = 7;
int red = 8;
int a;
int Steps = 0;
bool Direction = true;
unsigned long last_time;
unsigned long currentMillis ;
int steps_left; // 4095 for full 360 deg
long time;
int setAngle = 90;

RFID rfid(SS_PIN,RST_PIN);






//This is your card's UID which the system recognizes so it opens the door.
// CHANGE THE VALUEs ACCORDING TO YOUR CARD's UID! These are my cards' UID, so it'll not work with your cards.
// int cards[][x]; x represents the number of different amount of accepted cards.
int cards[][2] = {
  {215,214,231,117,147},  //Card
  {136,4,177,228,217} // Card2
};





String names[] = {"Card","Card2"};

bool access = false;
 
void setup(){
    lcdScreen.begin(20,4);
    lcdScreen.clear();
    Serial.begin(9600);
    Serial.print("===RFID DOOR LOCK by EugeneOnTheEdge===\n\nInitializing system and sensors...");
    delay(1250);
    
    buzzer.begin(45);
    SPI.begin(); //SPI MUST ALWAYS COMES FIRST,
    rfid.init(); // RFID.INIT 2ND ; otherwise an error will occur (which I still dunno why)

    pinMode(red,OUTPUT); 
    pinMode(green,OUTPUT);
    pinMode(yellow,OUTPUT);
    
    //-----This is just for the sake of 'fun' and yeah-----
    for (int xi = 0; xi < 3 ; xi++ )
    {
      digitalWrite(red,HIGH);
      delay(150);
      digitalWrite(yellow,HIGH);
      delay(150);
      digitalWrite(green,HIGH);
      delay(150);
      digitalWrite(red,LOW);
      delay(150);
      if (xi == 2)
      {
        int serWarning = 0;
        pinMode(IN1, OUTPUT); 
        pinMode(IN2, OUTPUT); 
        pinMode(IN3, OUTPUT); 
        pinMode(IN4, OUTPUT); 
        
        while (!Serial)
        {
          if (serWarning % 2 == 0)
          {
            digitalWrite(red,HIGH);
            buzzer.play(500,100);
            delay(200);
            digitalWrite(red,LOW);
            digitalWrite(yellow,HIGH);
            buzzer.play(750,100);
            delay(200);
            digitalWrite(yellow,LOW);
            digitalWrite(green,HIGH);
            buzzer.play(500,100);
            delay(200);
            digitalWrite(green,LOW);
            buzzer.play(750,100);
            delay(1000);
            serWarning++;
             //LED Animation
          }
          
          else
          {
            digitalWrite(red,HIGH);
            delay(200);
            digitalWrite(red,LOW);
            digitalWrite(yellow,HIGH);
            delay(200);
            digitalWrite(yellow,LOW);
            digitalWrite(green,HIGH);
            delay(200);
            digitalWrite(green,LOW);
            delay(1000);
            serWarning++;
          }
          
        }
      }
      digitalWrite(yellow,LOW);
      delay(150);
      digitalWrite(green,LOW);
      delay(150);
    }
    
    buzzer.play(500,250);
    digitalWrite(red,HIGH);
    delay(125);
    digitalWrite(yellow,HIGH);
    buzzer.play(750,250);
    delay(125);
    digitalWrite(green,HIGH);
    buzzer.play(1000,250);
    delay(1000);
    
    //------------------------------------------
    
    digitalWrite(green,LOW);
    delay(150);
    digitalWrite(yellow,LOW);
    delay(150);
    digitalWrite(red,LOW);
    delay(150);
    Serial.print("\n\nSystem ready!\n\nScan RFID card to unlock door.");
    
    
}
 
void loop(){

    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,HIGH);
    int x = 0;
    int i = 0;
    
    rfid.init();
    
    lcdScreen.clear();
    lcdScreen.setCursor(3,0);
    lcdScreen.print("RFID ID Reader");
    lcdScreen.setCursor(3,1);
    lcdScreen.print("Ready to scan.");
    
    if(rfid.isCard()){
    
        if(rfid.readCardSerial())
        {
            Serial.print("\n\nCard UID: ");
            
            Serial.print(rfid.serNum[0]);
            Serial.print(" ");
            Serial.print(rfid.serNum[1]);
            Serial.print(" ");
            Serial.print(rfid.serNum[2]);
            Serial.print(" ");
            Serial.print(rfid.serNum[3]);
            Serial.print(" ");
            Serial.print(rfid.serNum[4]);
            Serial.println(" ");
            
            for(x = 0; x < sizeof(cards); x++)
            {
              for(i = 0; i < sizeof(rfid.serNum); i++ )
              {
                  if(rfid.serNum[i] != cards[x][i]) 
                  {
                      access = false;
                      break;
                  } 
                  else 
                  {
                      access = true;
                  }
              }
              if(access) break;
            }
           
        }
        
       if(access){
          buzzer.play(500,100);
          delay(100);
          buzzer.play(750,250);
          delay(100);
          lcdScreen.clear();
          digitalWrite(green,HIGH);
          Serial.print("Welcome, ");  
          Serial.print(names[x]); 
          delay(250);
          turnMotor(90);

          digitalWrite(red,LOW);
          digitalWrite(green,LOW);
          Direction = ! Direction;
          digitalWrite(yellow,LOW);              
          delay(250);
          for (int blinks = 1; blinks <=5; blinks++)
          {
            digitalWrite(green,HIGH);
            delay(125);
            digitalWrite(green,LOW);
            delay(125);
          }
          digitalWrite(yellow,LOW);
      }
       
      else
      {
           Serial.print("Card not recognized!");
           lcdScreen.print("Card not recognized!");  
           digitalWrite(red,HIGH); 
           buzzer.play(750,250);
           delay(125);
           digitalWrite(red,LOW);
           buzzer.play(750,250);
           delay(125);
           digitalWrite(red,HIGH);
           buzzer.play(750,250);
           delay(125);
           digitalWrite(red,LOW);
           lcdScreen.clear();
           lcdScreen.setCursor(0,2);
           lcdScreen.print("Card UID: ");
           lcdScreen.setCursor(0,3);
           for ( a = 0 ; a <= 4 ; a++ )
           {
              lcdScreen.print(rfid.serNum[a]);
              lcdScreen.print(" ");
           }

           delay(1500); 
           digitalWrite(red,LOW);
           digitalWrite(green,LOW);
       }        
    }
    rfid.halt();
}
