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
int JoyThrtl_Con  = A1;
int JoyYaw_Con    = A0;
int JoyPitch_Con  = A2;
int JoyRoll_Con   = A3;

int Rot_LeftPin      = A8;
int Rot_RightPin     = A9;

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
int JoyYaw_Pos    = 0;
int JoyPitch_Pos  = 0;
int JoyRoll_Pos   = 0;

int Throttle      = 0;
int Yaw           = 0;
int Roll          = 0;
int Pitch         = 0;

int Rot_Left      = 0;
int Rot_Right      = 0;

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

  //lcd.init();
  //lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
//  updateLCD();
 pollJoy();
 mapJoy();
 readRot();
 //read_shift_regs();
 updatePullRegister();
    
  // Print to serial monitor
  Serial.print(incoming);
  Serial.println(incoming2);
  delay(200);

}

void mapJoy(){
  Throttle = map(JoyThrtl_Pos, 107, 892, 0, 511); //0 at throttle off, 511 at max throttle
  Yaw = map(JoyYaw_Pos, 150, 880, 0, 511);        //0 at full left, 511 at full right
  Pitch = map(JoyPitch_Pos, 120, 838, 0, 511);    //0 at nose down, 511 at nose up
  Roll = map(JoyRoll_Pos, 125, 880, 0, 511);      //0 at bank right, 511 at bank left
}

void pollJoy(){
  JoyThrtl_Pos = constrain(analogRead(JoyThrtl_Con), 107, 892); //center at N/A
  JoyYaw_Pos = constrain(analogRead(JoyYaw_Con), 150, 880);     //center at 513-518
  if (abs(515 - JoyYaw_Pos) <= 10) {                            //add deadzone so around center is always 515
    JoyYaw_Pos = 515;
  }
  JoyPitch_Pos = constrain(analogRead(JoyPitch_Con), 120, 838); //center at 469-488
  if (abs(475 - JoyPitch_Pos) <= 10) {
    JoyPitch_Pos = 479;
  }
  JoyRoll_Pos = constrain(analogRead(JoyRoll_Con), 125, 851);   //center at 487-488
  if (abs(488 - JoyRoll_Pos) <= 10) {
    JoyRoll_Pos = 488;
  }
}

void readRot(){
  Rot_Left  = analogRead(Rot_LeftPin);
  Rot_Right = analogRead(Rot_RightPin);
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

void updatePullRegister(){
  digitalWrite(clockEnablePin, HIGH); //First step here will be to pulse the latch/reset pin while sending a falling clock signal.
  digitalWrite(load, LOW); //This will lock in the current state of the inputs to be sent to the arduino. 
  delayMicroseconds(5);
  digitalWrite(clockEnablePin, LOW);
  digitalWrite(load, HIGH); 

  for(int i = 0;i<8;i++){
    bitWrite(incoming, i, digitalRead(dataIn)); //Grab our byte, select which bit, and write the current input as 1 or 0

    digitalWrite(clockEnablePin, LOW); 
    delayMicroseconds(5);
    digitalWrite(clockEnablePin, HIGH); //send a clock leading edge so to load the next bit
  }
    for(int i = 0;i<8;i++){
    bitWrite(incoming2, i, digitalRead(dataIn)); //Grab our byte, select which bit, and write the current input as 1 or 0

    digitalWrite(clockEnablePin, LOW); 
    delayMicroseconds(5);
    digitalWrite(clockEnablePin, HIGH); //send a clock leading edge so to load the next bit
  }

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
