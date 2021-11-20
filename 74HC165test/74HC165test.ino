/*
  74HC165 Shift Register Demonstration 1
  74hc165-demo.ino
  Read from 8 switches and display values on serial monitor

  DroneBot Workshop 2020
  https://dronebotworkshop.com
*/

// Define Connections to 74HC165

// PL pin 1
int load = 1;
// CE pin 15
int clockEnablePin = 4;
// Q7 pin 7
int dataIn = 5;
// CP pin 2
int clockIn = 6;

byte incoming = 0;
byte incoming2 = 0;

void setup()
{

  // Setup Serial Monitor
  Serial.begin(9600);

  // Setup 74HC165 connections
  pinMode(load, OUTPUT);
  pinMode(clockEnablePin, OUTPUT);
  pinMode(clockIn, OUTPUT);
  pinMode(dataIn, INPUT);
}

void loop()
{
  //get_Buttons();
  updatePullRegister();

  // Print to serial monitor
  Serial.print("Pin States:\r\n");
  Serial.print(incoming, BIN);
  Serial.println(incoming2, BIN);
  delay(400);
}


void get_Buttons(){
// Write pulse to load pin
  digitalWrite(load, LOW);
  delayMicroseconds(5);
  digitalWrite(load, HIGH);
  delayMicroseconds(5);

  // Get data from 74HC165
  digitalWrite(clockIn, HIGH);
  digitalWrite(clockEnablePin, LOW);
  incoming = shiftIn(dataIn, clockIn, MSBFIRST);
  incoming2 = shiftIn(dataIn, clockIn, MSBFIRST);
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
