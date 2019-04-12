//**************************************************************//
//  Name    :Panell Deco Imagenio                        //
//  Author  : Toniuep, toniuep@easyrobotics.es                              //
//  Date    : 04 Juny, 2018                                      //
//  Version : 1.0                                               //
//  Notes   : Code for Test the use of a PT6958 LED Controller          //
//          : of the imagenio decoders front panel.                        //
//****************************************************************
#include <IRremote.h>

int RECV_PIN = 2;

IRrecv irrecv(RECV_PIN);

decode_results results;
//Pin connected to ST_CP
int latchPin = 8;
//Pin connected to SH_CP
int clockPin = 12;
////Pin connected to DS
int dataPin = 11;



int RST = 3;
byte ds[10];
byte ds2[10];
byte dataRead[3];

byte switchVar1 = 72;  //01001000

int s1 = 0;
int s2 = 0;
byte num[] = {0b00111111,//0
              0b00000110,//1
              0b01011011,//2
              0b01001111,//3
              0b01100110,//4
              0b01101101,//5
              0b01111101,//6
              0b00000111,//7
              0b01111111,//8
              0b01101111,//9
             };

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  Serial.begin(38400);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
  
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  pinMode(RST, INPUT);
  

  ds[0] = 0b00111111;//SG1
  ds[1] = 0b00000000;//L1
  ds[2] = 0b00111111;//SG2
  ds[3] = 0b00000000;//L2
  ds[4] = 0b00111111;//SG3
  ds[5] = 0b00000000;//L3
  ds[6] = 0b00111111;//SG4
  ds[7] = 0b00000000;//L4
  ds[8] = 0b00000011;//Punts
  ds[9] = 0b00000000;//L5

  ds2[0] = 0b00000000;//SG1
  ds2[1] = 0b00000000;//L1
  ds2[2] = 0b00000000;//SG2
  ds2[3] = 0b00000000;//L2
  ds2[4] = 0b00000000;//SG3
  ds2[5] = 0b00000001;//L3
  ds2[6] = 0b00000000;//SG4
  ds2[7] = 0b00000001;//L4
  ds2[8] = 0b00000000;//Punts
  ds2[9] = 0b00000001;//L5



  digitalWrite(latchPin, HIGH);
  delay(200);
  command1();
  command2(ds);
  command3();
  setReadMode();
  //getReadData();
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value );
    if(results.value == 16724175){
       ds[5] = 0b00000001; 
      }
     if(results.value == 16718055){
       ds[7] = 0b00000001;
    }
     if(results.value == 16743045){
       ds[9] = 0b00000001;
    }
    
     if(results.value == 16716015){
      ds[5] = 0b00000010;
    }
     if(results.value == 16726215){
      ds[7] = 0b00000010;
    }
     if(results.value == 16734885){
      ds[9] = 0b00000010;
    }
    
    
    irrecv.resume(); // Receive the next value
  }
  delay(0);
  if (Serial.available() > 0) {

  }
 
  for (int i = 0; i < 3; i++) {
    dataRead[i] = shiftIn_(11, 12);
    //Serial.println(dataRead[i], DEC);

    if (dataRead[i] == 32) {
       s1 = 0;
      s2 = 0;
      command1();
      command2(ds2);
      command3();
      setReadMode();
      

    }else{
     
      setReadMode();
      rellotge();
    }
      

    
  }
  //white space
  //Serial.println("-------------------");
  //delay so all these print satements can keep up.
  //delay(500);
}
void rellotge() {
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if (s1 < 9) {
      s1++;
    } else {
      if (s2 < 9) {
        s2++;
      } else {
        s2 = 0;
      }

      s1 = 0;

    }

    ds[6 ] = num[s1];
    ds[4] = num[s2];
    
    command1();
    command2(ds);
    command3();
    setReadMode;
  }
}









void setReadMode() {
  byte cmd = 0b01000010;

  digitalWrite(latchPin, LOW);
  shiftOut_(dataPin, clockPin, cmd);
  //digitalWrite(latchPin, HIGH);

}


void command1() {

  byte cmd = 0b01000000;
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  shiftOut_(dataPin, clockPin, cmd);
  digitalWrite(latchPin, HIGH);
  delay(10);
}
void command2(byte data[]) {

  byte cmd   = 0b11000000;

  digitalWrite(latchPin, LOW);
  shiftOut_(dataPin, clockPin, cmd);
  for (int i = 0; i < 10; i++) {
    shiftOut_(dataPin, clockPin, data[i]);
  }
  digitalWrite(latchPin, HIGH);

}

void command3() {
  byte cmd = 0b10001111;

  digitalWrite(latchPin, LOW);
  shiftOut_(dataPin, clockPin, cmd);

  digitalWrite(latchPin, HIGH);
}

// the heart of the program
void shiftOut_(int myDataPin, int myClockPin, byte myDataOut) {
  int i = 0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  for (i = 0; i <= 7; i++)  {
    digitalWrite(myClockPin, 0);

    if ( myDataOut & (1 << i) ) {
      pinState = 1;
    }
    else {
      pinState = 0;
    }

    digitalWrite(myDataPin, pinState);

    digitalWrite(myClockPin, 1);

    digitalWrite(myDataPin, 0);
  }


  digitalWrite(myClockPin, 0);
}

byte shiftIn_(int myDataPin, int myClockPin) {
  int i;
  int temp = 0;
  int pinState;
  byte myDataIn = 0;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, INPUT);
  for (i = 0; i <= 7; i++)
  {
    digitalWrite(myClockPin, 0);
    delayMicroseconds(0.2);
    temp = digitalRead(myDataPin);
    if (temp) {
      pinState = 1;

      myDataIn = myDataIn | (1 << i);
    }
    else {
      pinState = 0;
    }
    digitalWrite(myClockPin, 1);

  }
  return myDataIn;
}
