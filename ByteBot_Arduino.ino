#include <SoftwareSerial.h>

//Miscalleneous Values
int pinTemp = A0;
const int trigPin = 9;  
const int echoPin = 6;
int motorSpeed = 0;       //starting speed for the motor
float duration, distance;
bool start = false;
bool lghtOn = 0;
int fantime;

//light sensor connected to analog pin a2 //****EDITED NEW

//candy vals
int state = 0;
int lightPin = A2;

const int EIN2 = 1; 
const int PWME = 4; 

const int AIN2 = 12;          //control pin 2 on the motor driver for the right motor
const int PWMA = 11;          //speed control pin on the motor driver for the right motor
const int BIN2 = 8;           //control pin 2 on the motor driver for the right motor
const int PWMB = 10;          //speed control pin on the motor driver for the right motor

const int CIN2 = 7;           //control pin 2 on the motor driver for the right motor
const int PWMC = 5;           //speed control pin on the motor driver for the right motor
const int DIN2 = 2;           //control pin 2 on the motor driver for the right motor
const int PWMD = 3;           //speed control pin on the motor driver for the right motor

void setup() {    
  setupDistSensor();
  setupMotor();
  pinMode(13, OUTPUT);
  Serial.begin(9600); //no need to set up light sensor, on analog pin
}

void loop() {
  startapp();
  systemsgo();
  Serial.print("&");
  Serial.println();
  delay (10);
 }  

 void startapp(){
  if (start == false){
      distSense();
      delay(10);
      digitalWrite(13, LOW);
    if (distance < 5){
      start = true;
    }
  }
}

void systemsgo(){
   if (start == true){
     light();
     Serial.print("candyState: ");
     Serial.println(state);
     if (state == 1) {
      for (int i = 0; i < 4; i++){
         // motor1;
         Serial.print("motor1");
         digitalWrite(13, HIGH);
         UseAMotor(1);
         temp();
         resetProgram();
        }
     }
      else if(state == 2){
        for (int i = 0; i < 4; i++){
         // motor2;
         Serial.print("motor2");
         digitalWrite(13, HIGH);
         UseAMotor(2);
         temp();
         resetProgram();
        }
      }
      else if(state == 3){
        for (int i = 0; i < 4; i++){
         // motor3;
         Serial.print("motor3");
         digitalWrite(13, HIGH);
         UseAMotor(3);
         temp();
         resetProgram();
        }
      }
      else if (state == 4){
        for (int i = 0; i < 4; i++){
        //  motor4;
        Serial.print("motor4");
        digitalWrite(13, HIGH);
        UseAMotor(4);
        temp();
        resetProgram();
        }
      }   
  }
}

void resetProgram() {
  start = false; 
  state = 0;
}


void temp(){
  int temp = analogRead(pinTemp);    //Read the analog pin
 // temp = temp * 0.01828125;   // convert output (mv) to readable celcius**USE RAW VALUES INSTEAD, DECIMALS DONT DETECT CHANGES
  Serial.print("t");
  Serial.print(temp);
  Serial.print("t");
  Serial.println();

  //check if threshold met
  if (temp>=350) { //859 + overheating **EDIT THIS LINE AND THE ONE BELOW IT DEPENDING ON CURRENT AMBIENT TEMPERATURE
    fanStatus(1);
  }
  if (temp<= 340) {
    fanStatus(2);
  }
}


void light() {
  int lightVal = analogRead(lightPin); //returns value from the lightSensor in range from 0-1023** check when you put this line
  Serial.print("l");
  Serial.print(lightVal);
  Serial.print("l");
  Serial.println();
  
  //assign thresholds of light brightness to a certain candy

  if (state == 0) { //CHECK THIS LINE, CHECKS ONLY IF ALL CANDY STILL FALSE, HASNT DETECTED ONE YET
      if (lightVal >= 17 && lightVal<= 33) {
      //candy1 = true; EDIGT THIS AGAIN LATER
        state = 1;
      }
      
      else if (lightVal>= 34 && lightVal<= 40) {
      //  candy2= true; REPLACE WITH LINE 162 LATER
        state = 2;
      }
  
      else if (lightVal>= 41 && lightVal <= 51) {
        state = 3;
      }
      else if (lightVal >= 53 && lightVal <= 100) {
        state = 4;
      }
  }
}

void distSense(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("d");
  Serial.print(distance);
  Serial.print("d");
  Serial.println();
}

void setupMotor() {
  //set the motor contro pins as outputs
  pinMode(EIN2, OUTPUT); //fan
  pinMode(PWME, OUTPUT);
  
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  
  pinMode(CIN2, OUTPUT);
  pinMode(PWMC, OUTPUT);
  pinMode(DIN2, OUTPUT);
  pinMode(PWMD, OUTPUT);
}

void setupDistSensor(){
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);
}

// New Motors
int UseAMotor(int x){
  if (x == 3){
    digitalWrite(AIN2, HIGH);                          //set pin 2 to low
    analogWrite(PWMA, 255);               //now that the motor direction is set, drive it at max speed
    delay(180);
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
    analogWrite(PWMA, 0);
    delay(5000);
  } 
  if (x == 4){
    digitalWrite(BIN2, HIGH);                          //set pin 2 to low
    analogWrite(PWMB, 255);               //now that the motor direction is set, drive it at max speed
    delay(180);
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
    analogWrite(PWMB, 0);
    delay(5000);
  } 
  if (x == 2){
    digitalWrite(CIN2, HIGH);                          //set pin 2 to low
    analogWrite(PWMC, 255);               //now that the motor direction is set, drive it at max speed
    delay(180);
    digitalWrite(CIN2, LOW);                          //set pin 2 to low
    analogWrite(PWMC, 0);
    delay(5000);
  } 
  if (x == 1){
    digitalWrite(DIN2, HIGH);                          //set pin 2 to low
    analogWrite(PWMD, 255);               //now that the motor direction is set, drive it at max speed
    delay(180);
    digitalWrite(DIN2, LOW);                          //set pin 2 to low
    analogWrite(PWMD, 0);
    delay(5000);
  } 
}

int fanStatus(int x){
  if (x == 1){
    Serial.print("fanstatus on");
    digitalWrite(EIN2, HIGH);                         //set pin 2 to high
    digitalWrite(PWME, HIGH);               //now that the motor direction is set, drive it at max speed
  }
  if (x == 2){
    Serial.print("fanstatus off");
    digitalWrite(EIN2, LOW);                         //set pin 2 to high
    digitalWrite(PWME, LOW);               //now that the motor direction is set, drive it at max speed
  }
}
