#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}
void setup(){
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  setDS3231time(0,18,20,2,29,1,19);
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime(){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  lcd.setCursor(0,0);
  lcd.print(hour,DEC);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  lcd.setCursor(1,0);
  Serial.print(":");
  lcd.print(":");
  if (minute<10){
    Serial.print("0");
    
    lcd.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  lcd.setCursor(3,0);
  lcd.print(minute, DEC);
  lcd.setCursor(4,0);
  lcd.print(":");
  if (second<10){
    Serial.print("0");
    lcd.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  lcd.print(second,DEC);
  lcd.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  lcd.print(dayOfMonth,DEC);
  lcd.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  lcd.print(month,DEC);
  lcd.print("/");
  Serial.print(year, DEC);
  lcd.print(year,DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    lcd.print("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    lcd.print("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    lcd.print("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    lcd.print("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    lcd.print("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    lcd.print("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    lcd.print("Saturday");
    break;
  }
}
void loop(){
  displayTime(); // display the real-time clock data on the Serial Monitor,
  delay(1000); // every second
}
