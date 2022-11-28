/* 
Setup:
- Initialize LCD
- Initialize Clock
- Initialize relay
- Set Time (time already set here)
- Preset Time - 19.00
- Preset Off Time - 7.00

Loop:
- Get Current time
- If time is equal or higher than preset on time, turn on Lights
- Else turn off lights
- calculate time the system will go to sleep for
- sleep for allotted time
*/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

LiquidCrystal_I2C lcd(0x27,20,4);
int relayPin = 2;
uint8_t presetOnTime = 19; // 7pm
uint8_t presetOffTime = 7; // 7am
int cleared = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  tmElements_t tm;
  if (RTC.read(tm)) {
    lcd.setCursor(0,0);
    lcd.print("Time:");
    print2digits(tm.Hour);
    lcd.print(":");
    print2digits(tm.Minute);
    lcd.print(":");
    print2digits(tm.Second);
  } else {
    if (RTC.chipPresent()) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error. Reset");
      lcd.setCursor(0,1);
      lcd.print("RTC");
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  lcd.setCursor(0,1);
  if (tm.Hour >= presetOnTime || tm.Hour < presetOffTime){
    if (!cleared) clearline(1);
    digitalWrite(relayPin, HIGH);
    lcd.print("Status: ON");
    }
   else {
    digitalWrite(relayPin, LOW);
    lcd.print("Status: OFF");
    cleared = 0;
   }
  delay(1000);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.print('0');
  }
  lcd.print(number);
}

void clearline(int line){
  lcd.setCursor(0, line);
  for (int i = 0; i < 16; i++){
    lcd.write(' ');
    } 
    cleared = 1; 
}
