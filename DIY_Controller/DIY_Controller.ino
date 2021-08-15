// Arduino layout with connections
// Radio            MISO MOSI SCLK 2pin   pins 14+15+16+10+7  5V+GND
// Joystick L       A0, A1                pins 18+19          5V+GND
// Joystick R       A2, A3                pins 20+21          5V+GND
// 2 rotaries       A8, A9                pins 8+9            5V+GND
// "D-pad" buttons  To SN74HC165N
// "ABXY" buttons   To SN74HC165N
// 2 top buttons    To remaining pin, last button unable (Maybe daisychain two SN74HC165N together to be able to fit all buttons tenzij CE niet geconnect hoeft te worden op de SN74HC165N, dan blijven er twee pins over op de arduino voor de knoppen.
// Display          SDA,SCL               pins 2+3            5V+GND
// Battery          N/A                   N/A                 5V+GND

// 6x 5V+GND connection

// pins left, 4,5,6,0,1 (5 pins for 10 buttons -> needs input encoder thing)
// SN74HC165N 8buttons to serial connection (Dpad+ABXY)    latch,clock,data     5V+GND



#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);



void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  updateLCD();
}

void updateLCD(){
  lcd.setCursor(2,0); //3rd column 1st row
  lcd.print("Hoi Maureeeeen");
  lcd.setCursor(2,1);
  lcd.print("<3 <3 <3 <3");
  lcd.noCursor();
  delay(1000);
  lcd.clear();
  delay(1000);
}
