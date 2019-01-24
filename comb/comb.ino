#include "Wire.h"
#include <LiquidCrystal_I2C.h>
#include "dht.h"
#define DS3231_I2C_ADDRESS 0x68
#define dht_apin A0 // Analog Pin sensor is connected to
dht DHT;
LiquidCrystal_I2C lcd(0x27,16,2);


byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
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
  lcd.print((int)hour/10);
  lcd.setCursor(1,0);
  lcd.print((int)hour%10);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  lcd.setCursor(2,0);
  Serial.print(":");
  lcd.print(":");
  if (minute<10){
    Serial.print("0");
    lcd.setCursor(3,0);
    lcd.print("0");
    lcd.setCursor(4,0);
    lcd.print((int)minute%10);  
  }
  
  else{
  lcd.setCursor(3,0);
  lcd.print((int)minute/10);
  lcd.setCursor(4,0);
  lcd.print((int)minute%10);
  }
  lcd.setCursor(5,0);
  lcd.print(":");
  if(second<10){
    
    Serial.print("0");
    lcd.setCursor(6,0);
    lcd.print("0");
    lcd.setCursor(7,0);
    lcd.print((int)second%10);  
  }
  
  else{
  lcd.setCursor(6,0);
  lcd.print((int)second/10);
  lcd.setCursor(7,0);
  lcd.print((int)second%10);
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  
  lcd.setCursor(9,0);
  lcd.print((int)dayOfMonth/10);
  lcd.setCursor(10,0);
  lcd.print((int)dayOfMonth%10);
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  lcd.setCursor(12,0);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    lcd.print("S");
    lcd.setCursor(13,0);
    lcd.print("U");
    lcd.setCursor(14,0);
    lcd.print("N");
    break;
  case 2:
    Serial.println("Monday");
    lcd.print("M");
    lcd.setCursor(13,0);
    lcd.print("O");
    lcd.setCursor(14,0);
    lcd.print("N");
    break;
  case 3:
    Serial.println("Tuesday");
    lcd.print("T");
    lcd.setCursor(13,0);
    lcd.print("U");
    lcd.setCursor(14,0);
    lcd.print("E");
    break;
  case 4:
    Serial.println("Wednesday");
    lcd.print("W");
    lcd.setCursor(13,0);
    lcd.print("E");
    lcd.setCursor(14,0);
    lcd.print("D");
    break;
  case 5:
    Serial.println("Thursday");
    lcd.print("T");
    lcd.setCursor(13,0);
    lcd.print("H");
    lcd.setCursor(14,0);
    lcd.print("U");
    break;
  case 6:
    Serial.println("Friday");
    lcd.print("F");
    lcd.setCursor(13,0);
    lcd.print("R");
    lcd.setCursor(14,0);
    lcd.print("I");
    break;
  case 7:
    Serial.println("Saturday");
    lcd.print("S");
    lcd.setCursor(13,0);
    lcd.print("A");
    lcd.setCursor(14,0);
    lcd.print("T");
    break;
  }
}


void setup() {
  // put your setup code here, to run once:
      lcd.init(); //initialize the lcd
      lcd.backlight(); //open the backlight
      Serial.begin(9600);
      delay(500);//Delay to let system boot
      Serial.println("DHT11 Humidity & temperature Sensor\n\n");
      delay(1000);//Wait before accessing Sensor 
}


void loop() {
  // put your main code here, to run repeatedly:
    DHT.read11(dht_apin);
    
    
    
    
    lcd.setCursor(2,1);
    lcd.print("H");
    lcd.setCursor(3,1);
    lcd.print("=");
    lcd.setCursor(4,1);
    lcd.print((int)DHT.humidity/10);
    lcd.setCursor(5,1);
    lcd.print((int)DHT.humidity%10);
    lcd.setCursor(6,1);
    lcd.print("%");

    lcd.setCursor(9,1);
    lcd.print("T");
    lcd.setCursor(10,1);
    lcd.print("=");
    lcd.setCursor(11,1);
    lcd.print((int)DHT.temperature/10);
    lcd.setCursor(12,1);
    lcd.print((int)DHT.temperature%10);
    lcd.setCursor(13,1);
    lcd.print("C");
    
    
     Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
    displayTime();
    delay(1000);
}
