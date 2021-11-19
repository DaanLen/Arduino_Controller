// Arduino layout with connections
// Radio            MISO MOSI SCLK 2pin   pins 14+15+16+10+7  5V+GND    CSN ->7 CE ->10
// Joystick L       A0, A1                pins 18+19          5V+GND
// Joystick R       A2, A3                pins 20+21          5V+GND
// 2 rotaries       A8, A9                pins 8+9            5V+GND
// "D-pad" buttons  To SN74HC165N
// "ABXY" buttons   To SN74HC165N
// 2 top buttons    To SN74HC165N
// Display          SDA,SCL               pins 2+3            5V+GND
// Battery          N/A                   N/A                 5V+GND

// 6x 5V+GND connection

// pins left, 4,5,6,0,1 (5 pins for 10 buttons -> needs input encoder thing)
// SN74HC165N 8buttons to serial connection (Dpad+ABXY)    latch,clock,data     5V+GND

//JoyStick config max ranges
//Throttle [106 893]
//Yaw [139 918]


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); //Initialise LCD Connection (needs to be connected to I2C pins SDA SCL which are pins 2 and 3 respectively on Pro Micro)

//Define Pins
int JoyThrtl_Con  = A0;
int JoyYaw_Con    = A1;
int JoyPitch_Con  = A2;
int JoyRoll_Con   = A3;

// Define Connections to 74HC165
// PL pin 1
int load = 1;
// CE pin 15
int clockEnablePin = 4;
// Q7 pin 9
int dataIn = 5;
// CP pin 2
int clockIn = 6;

//Set initial values
int JoyThrtl_Pos  = 0;
int JoyYaw_Pos   = 0;
int JoyPitch_Pos  = 0;
int JoyRoll_Pos   = 0;

int Throttle      = 0;
int Yaw           = 0;
int Roll          = 0;
int Pitch         = 0;

byte incoming = 0;
byte incoming2 = 0;

void setup() {
  Serial.begin(9600);
  //Setup Joystick connections
  pinMode(JoyThrtl_Con, INPUT);
  pinMode(JoyYaw_Con, INPUT);
  pinMode(JoyPitch_Con, INPUT);
  pinMode(JoyRoll_Con, INPUT);

  // Setup 74HC165 connections
  pinMode(load, OUTPUT);
  pinMode(clockEnablePin, OUTPUT);
  pinMode(clockIn, OUTPUT);
  pinMode(dataIn, INPUT);

  lcd.init();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
//  updateLCD();
//  pollJoy();
//  mapJoy();
  read_shift_regs();
  updateLCD();
    
  // Print to serial monitor
  Serial.print("Pin States:\r\n");
  Serial.print(incoming, BIN);
  Serial.println(incoming2, BIN);
  delay(200);

}

void mapJoy(){
  Throttle = map(JoyThrtl_Pos, 107, 892, 0, 511); //0 at throttle off, 511 at max throttle
  Yaw = map(JoyYaw_Pos, 150, 900, 0, 511);        //0 at full left, 511 at full right
  Pitch = map(JoyPitch_Pos, 120, 885, 0, 511);    //0 at nose down, 511 at nose up
  Roll = map(JoyRoll_Pos, 125, 880, 0, 511);      //0 at bank right, 511 at bank left
}

void pollJoy(){
  JoyThrtl_Pos = constrain(analogRead(JoyThrtl_Con), 107, 892); //center at N/A
  JoyYaw_Pos = constrain(analogRead(JoyYaw_Con), 150, 900);     //center at 245-250
  JoyPitch_Pos = constrain(analogRead(JoyPitch_Con), 120, 885); //center at 242
  JoyRoll_Pos = constrain(analogRead(JoyRoll_Con), 125, 880);   //center at 246
}


void read_shift_regs(){
    // Write pulse to load pin
  digitalWrite(load, LOW);
  delayMicroseconds(5);
  digitalWrite(load, HIGH);
  delayMicroseconds(5);

  // Get data from 74HC165
  digitalWrite(clockIn, HIGH);
  digitalWrite(clockEnablePin, LOW);
  incoming = shiftIn(dataIn, clockIn, LSBFIRST);
  incoming2 = shiftIn(dataIn, clockIn, LSBFIRST);
  digitalWrite(clockEnablePin, HIGH);
}


void updateLCD(){
  lcd.setCursor(2,0); //3rd column 1st row
  lcd.print("Test message");
  lcd.setCursor(2,1);
  lcd.print("testing");
  lcd.noCursor();
  delay(1000);
  lcd.clear();
  delay(1000);
}
