#include <Servo.h>
#include "pitches.h"


const int latchPin = 10;
const int clockPin = 12;
const int dataPin = 11;

#define pinLeftWheel            8   
#define pinRightWheel           9 
#define wheelStopValue 90
#define leftWheelFordwardValue 0
#define leftWheelBackwardsValue 180
#define rightWheelFordwardValue 180
#define rightWheelBackwardsValue 0
#define defaultDelay        10
#define bufferSize 5

byte data;
byte dataArray[10];

Servo leftWheel;                      
Servo rightWheel;   

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};


int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


char dataBuffer[bufferSize]; 
int i = 0;
int numChar = 0;    


void setup() {
  
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  
  Serial.begin(38400);
  
  leftWheel.attach(pinLeftWheel);
  rightWheel.attach(pinRightWheel);
 
  dataArray[0] = 0b11111111;
  dataArray[1] = 0b11111111;
  dataArray[2] = 0b11111101;
  dataArray[3] = 0b11111001;
  dataArray[4] = 0b11110001;
  dataArray[5] = 0b11100001;
  dataArray[6] = 0b11000001;
  dataArray[7] = 0b10000001;
  dataArray[8] = 0b00000001;
  dataArray[9] = 0b11111111;
  
  blinkAll_2Bytes(2,500); 
  stopWheels();
}


void loop() {
  if (Serial.available() > 0) { 
    i = 0;
    memset(dataBuffer, 0, sizeof(dataBuffer));  
    delay(bufferSize); 
    numChar = Serial.available();  
    
    if (numChar > bufferSize) {
          numChar = bufferSize;
    }

    while (numChar--) {
        dataBuffer[i++] = Serial.read();
        delay(3);
    } 
    checkData(dataBuffer);
    
  }
  int val = digitalRead(3);
 if(val==HIGH){
  playSound();
 }
}

void playSound(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    int noteDuration = 1000 / noteDurations[thisNote];
    tone(2, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
   
    noTone(2);
  }
  for (int j = 0; j < 10; j++) {
    
    data = dataArray[j];
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data);
    digitalWrite(latchPin, 1);
    delay(300);
  }
}

 
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  int i=0;
  int pinState;

  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  for (i=7; i>=0; i--)  {
    if(digitalRead(3)){
     playSound();
  }
    digitalWrite(myClockPin, 0);

    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

  
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }

  digitalWrite(myClockPin, 0);
}

void blinkAll_2Bytes(int n, int d) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++) {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}

void stopWheels() {
  leftWheel.write(wheelStopValue+1);
  delay(defaultDelay);

  rightWheel.write(wheelStopValue-1);
  delay(defaultDelay);
}


void goForwards() {
  leftWheel.write(leftWheelFordwardValue);
  delay(defaultDelay);

  rightWheel.write(rightWheelFordwardValue);
  delay(defaultDelay);
}


void goBackwards() {
  leftWheel.write(leftWheelBackwardsValue);
  delay(defaultDelay);

  rightWheel.write(rightWheelBackwardsValue);
  delay(defaultDelay);
}


void goLeft() {

  leftWheel.write(wheelStopValue);
  delay(defaultDelay);

  rightWheel.write(rightWheelFordwardValue);
  delay(defaultDelay);
}


void goRight() {

  leftWheel.write(leftWheelFordwardValue);
  delay(defaultDelay);

  rightWheel.write(wheelStopValue);
  delay(defaultDelay);
}

void checkData(char* data){  
  
   switch(data[0]) {

    case'S':
      stopWheels();
      break;
    
    case 'F':
      goForwards();
      break;
    
    case 'B':
      goBackwards();
      break;
      
    case 'L':
      goLeft();
      break;
      
    case 'R':
      goRight();
      break;

      case 'H':
     playSound();
      break; 
    
  } 

  Serial.flush();

}
