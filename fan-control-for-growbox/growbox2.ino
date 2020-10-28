#include <EEPROM.h>
#include "Wire.h"
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(A3, DHT11);

unsigned long timer1Millis;
unsigned long timer2Millis;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long timer1 = 0;
unsigned long timer2 = 0;
int VRX;
int VRY;
bool b1 = 0;
bool b2 = 0;
bool b3 = 0;
bool b4 = 0;
bool menu = 0;
bool save = 0;
bool relay_state;

void setup() {
  relay_state = 1;
  EEPROM.get(0, timer1);
  EEPROM.get(4, timer2);
  
  pinMode(A2, INPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  dht.begin();
  lcd.init();                      
  lcd.backlight();
  lcd.clear();
  
  
  //Serial.begin(9600);
}

void loop() {

 
  
 //Serial.println(timer1);
//  Serial.println(sizeof(timer1)); 
//  Serial.println("timer2=");
//  Serial.println(sizeof(timer2));
  
  
  

  float h = dht.readHumidity(); //Измеряем влажность
  float t = dht.readTemperature(); //Измеряем температуру
  
  lcd.setCursor(0, 0);
  lcd.print("T=");
  lcd.setCursor(2, 0);
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("H=");  
  lcd.setCursor(2, 1);
  lcd.print(h);
  
  lcd.setCursor(8, 0);
  lcd.print("On");  
  lcd.setCursor(12,0);
  lcd.print(timer1);
  lcd.setCursor(8, 1);
  lcd.print("Off");  
  lcd.setCursor(12,1);
  lcd.print(timer2);
  
  if (save == 1){
    lcd.setCursor(15,0);
    lcd.print("*");
  }

  if (digitalRead(A2) == LOW){ 
    EEPROM.put(0,timer1);
    EEPROM.put(4,timer2);
    lcd.clear();
    save = 0;
  }
  joystick();
  menu0();
  relay();
  delay(30);
}

void joystick() {
  VRX = analogRead(A0);
  VRY = analogRead(A1);
  
  if (VRX<200){
    b1 = 1;
  }else if (VRX>800){
    b2 = 1;
  }else{
    b1 = 0;
    b2 = 0;
  }
  
  if (VRY<200){
    b3 = 1;
  }else if (VRY>800){
    b4 = 1;
  }else{
    b3 = 0;
    b4 = 0;
  }
}


void menu0() {
    if (b3 == 1){
      timer1 = timer1 + 5;
      timer1 = constrain(timer1, 0, 900);
      lcd.clear();
      save = 1;
    }else if (b4 == 1){
      timer1 = timer1 - 5;
      timer1 = constrain(timer1, 0, 900);
      lcd.clear();
      save = 1;
    }else if (b2 == 1){
      timer2 = timer2 + 5;
      timer2 = constrain(timer2, 0, 900);
      lcd.clear();
      save = 1;
    }else if (b1 == 1){
      timer2 = timer2 - 5;
      timer2 = constrain(timer2, 0, 900);
      lcd.clear();
      save = 1;
    }
    
}

void relay() {
  timer1Millis = timer1 * 1000;
  timer2Millis = timer2 * 1000;
  currentMillis = millis();
  if (relay_state == 1){
    if (currentMillis - previousMillis >= timer2Millis){
      previousMillis = currentMillis;
      digitalWrite(2, LOW);
      relay_state = 0;
    } 
  }else if (currentMillis - previousMillis >= timer1Millis){
    previousMillis = currentMillis;
    digitalWrite(2, HIGH);
    relay_state = 1;
  }
}
