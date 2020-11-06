#include <TM1637.h>
#include <Wire.h>
#include <DS3231.h>
#include <SPI.h>
#include <DigitLed72xx.h>

#define LOAD_PIN 10 // chipselect pin
#define NCHIP 1     // number of devices that can be controled

DigitLed72xx ld = DigitLed72xx(LOAD_PIN, NCHIP);
TM1637 tm1637(3, 2);

DS3231 Clock;
byte Year;
byte Month;
byte Date;
byte DoW;
byte Hour;
byte Minute;
byte Second;
RTClib myRTC;

int button=9;
 

void GetDateStuff(byte& Year, byte& Month, byte& Day, byte& DoW, 
    byte& Hour, byte& Minute, byte& Second) {
  // Call this if you notice something coming in on 
  // the serial port. The stuff coming in should be in 
  // the order YYMMDDwHHMMSS, with an 'x' at the end.
  boolean GotString = false;
  char InChar;
  byte Temp1, Temp2;
  char InString[20];

  byte j=0;
  while (!GotString) {
    if (Serial.available()) {
      InChar = Serial.read();
      InString[j] = InChar;
      j += 1;
      if (InChar == 'x') {
        GotString = true;
      }
    }
  }
  Serial.println(InString);
  // Read Year first
  Temp1 = (byte)InString[0] -48;
  Temp2 = (byte)InString[1] -48;
  Year = Temp1*10 + Temp2;
  // now month
  Temp1 = (byte)InString[2] -48;
  Temp2 = (byte)InString[3] -48;
  Month = Temp1*10 + Temp2;
  // now date
  Temp1 = (byte)InString[4] -48;
  Temp2 = (byte)InString[5] -48;
  Day = Temp1*10 + Temp2;
  // now Day of Week
  DoW = (byte)InString[6] - 48;   
  // now Hour
  Temp1 = (byte)InString[7] -48;
  Temp2 = (byte)InString[8] -48;
  Hour = Temp1*10 + Temp2;
  // now Minute
  Temp1 = (byte)InString[9] -48;
  Temp2 = (byte)InString[10] -48;
  Minute = Temp1*10 + Temp2;
  // now Second
  Temp1 = (byte)InString[11] -48;
  Temp2 = (byte)InString[12] -48;
  Second = Temp1*10 + Temp2;
}

void setup () {
    Serial.begin(57600);
    Wire.begin();
    tm1637.init();
    tm1637.setBrightness(5);
    ld.on(1);
    ld.setBright(9);
}

void loop () {
  if (Serial.available()) {
    GetDateStuff(Year, Month, Date, DoW, Hour, Minute, Second);
    
    Clock.setClockMode(false);  // set to 24h
    Clock.setYear(Year);
    Clock.setMonth(Month);
    Clock.setDate(Date);
    Clock.setDoW(DoW);
    Clock.setHour(Hour);
    Clock.setMinute(Minute);
    Clock.setSecond(Second);
}
    delay(1000);
    DateTime now = myRTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    tm1637.display(now.minute()+now.hour()*100, DEC);
    tm1637.switchColon();
    ld.printDigit(now.year()-2000, 1);
    ld.printDigit(now.month(), 1,2);
    if (long(now.month())>=0 and long(now.month())<9) 
 {
    ld.printDigit(0,1,3);
 }    
    ld.printDigit(now.day(),1,4);
    if (long(now.day())>=0 and long(now.day())<9) 
 {
    ld.printDigit(0,1,5);
 }        
    ld.printDigit(now.second(),1,6);
    if (long(now.second())>=0 and long(now.second())<9) 
 {
    ld.printDigit(0,1,7);
 }
}
