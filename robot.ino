
#include "MotorDriver.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 7); // RX, TX
String Data = "";
boolean auto_mode = false;
int acc = 20;
unsigned long echo = 0;
int ultraSoundSignal = 3; // Ultrasound signal pin
unsigned long ultrasoundValue = 0;

void setup() {
  motordriver.init();
  motordriver.setSpeed(200,MOTORB);
  motordriver.setSpeed(200,MOTORA);
  delay(100);
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(ultraSoundSignal,OUTPUT);
}

void loop() {
  if(auto_mode){
    int x = 0;
    x = ping();
    Serial.println(x);
    if(x < 6){
      stop_n_turn();
      delay(250);
    }
  }
  // put your main code here, to run repeatedly:
  if (mySerial.available()){
    if(acc>200)acc=200;
    char character = mySerial.read();
    Serial.write(character);
    Data.concat(character);
    if(Data == "a") {
        auto_mode = !auto_mode;
        Serial.print("Auto Mode: ");
        Serial.println(auto_mode);
    }
    if(Data == "u") {
        motordriver.setSpeed(acc,MOTORB);
        motordriver.setSpeed(acc,MOTORA);
        motordriver.goForward();
    }
    if(Data == "d"){
        motordriver.setSpeed(acc,MOTORB);
        motordriver.setSpeed(acc,MOTORA);
        motordriver.goBackward();
    }
    if(Data == "l"){
      if(acc > 110)acc = 10;
      if(acc > 100)acc = 100;
       motordriver.setSpeed(acc,MOTORB);
       motordriver.setSpeed(acc,MOTORA);
       motordriver.goRight();
       acc = acc-15;
    }
    if(Data == "1"){//FORWARD LEFT
       motordriver.setSpeed(acc/4,MOTORB);
       motordriver.setSpeed(acc,MOTORA);
       motordriver.goForward();
       acc = acc-15;
    }
    if(Data == "z"){//BACK LEFT
       motordriver.setSpeed(acc/4,MOTORB);
       motordriver.setSpeed(acc,MOTORA);
       motordriver.goBackward();
       acc = acc-15;
    }
    if(Data == "r"){
      if(acc > 110)acc = 10;
      if(acc > 100)acc = 100;
       motordriver.setSpeed(acc,MOTORB);
       motordriver.setSpeed(acc,MOTORA);
       motordriver.goLeft();
       acc = acc-15;
    }
    if(Data == "0"){//FORWARD RIGHT
       motordriver.setSpeed(acc,MOTORB);
       motordriver.setSpeed(acc/4,MOTORA);
       motordriver.goForward();
       acc = acc-15;
    }
    if(Data == "m"){//BACK RIGHT
       motordriver.setSpeed(acc,MOTORB);
       motordriver.setSpeed(acc/4,MOTORA);
       motordriver.goBackward();
       acc = acc-15;
    }
    if(Data == "x"){
       motordriver.stop();
       acc=20;
    }
    Serial.println(acc);
    acc = acc+20;
  }else{
    delay(200);
    if (mySerial.available()){
      //
    }else{
      acc=20;
      motordriver.stop();
    }
  }
  if (Serial.available())
    mySerial.write(Serial.read());
  Data = "";
}

void stop_n_turn(){
  acc = 0;
  motordriver.stop();
  delay(250);
  for(int i=10; i<=50; i+=10){
    motordriver.setSpeed(i,MOTORB);
    motordriver.setSpeed(i,MOTORA);
    motordriver.goLeft();
    delay(10);
  }
}

unsigned long ping(){
 pinMode(ultraSoundSignal, OUTPUT); // Switch signalpin to output
 digitalWrite(ultraSoundSignal, LOW); // Send low pulse
 delayMicroseconds(2); // Wait for 2 microseconds
 digitalWrite(ultraSoundSignal, HIGH); // Send high pulse
 delayMicroseconds(5); // Wait for 5 microseconds
 digitalWrite(ultraSoundSignal, LOW); // Holdoff
 pinMode(ultraSoundSignal, INPUT); // Switch signalpin to input
 digitalWrite(ultraSoundSignal, HIGH); // Turn on pullup resistor
  // please note that pulseIn has a 1sec timeout, which may
  // not be desirable. Depending on your sensor specs, you
  // can likely bound the time like this -- marcmerlin
  // echo = pulseIn(ultraSoundSignal, HIGH, 38000)
  echo = pulseIn(ultraSoundSignal, HIGH); //Listen for echo
  ultrasoundValue = (echo / 58.138) * .39; //convert to CM then to inches
 return ultrasoundValue;
 }
