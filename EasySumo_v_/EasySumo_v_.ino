//#include <SoftwareSerial.h>
//#include <SerialCommandOld.h>
#include <Servo.h>
#include <Ultrasonic.h>


//SerialCommand SCmd;   // The demo SerialCommand object
Servo r1, r2;
Ultrasonic US(12, 13);
boolean _start = false;

boolean enemyDetected = false;
boolean changedir = false;
int mode = 1;
int lastmode = 0;
int figth = 0;
void setup()
{
 Serial.begin(9600);
  r1.attach(10);
  r2.attach(11);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(7, INPUT);
  r1.write(90);
  r2.write(90);
  delay(4000);

}

void loop()
{
  int startButton = digitalRead(7);
  //Serial.println(startButton);
  if(startButton == 1){       
     if(figth == 0){
     figth = 1;
     }
     /*else{
      figth = 0;
     }
     delay(400);*/
  }
  //Serial.println(pitjat);
  
  if(figth){
  //SCmd.readSerial();     // We don't do much, just process serial commands
  int ir_1 = digitalRead(2);
  int ir_2 = digitalRead(3);

  int scanResult = US.distanceRead();
  delay(10);
  if (scanResult != 0 && scanResult < 40){
    mode = 1;
  }else{
    mode = 0;
  }

  if(lastmode != mode){
    if(mode == 0){
      Serial.println("Atack mode ON!!");
    }
    if(mode == 1){
      Serial.println("Searching for enemies....");
      Serial.print(changedir);
      changedir = !changedir;
    }
    lastmode = mode;
  }

  delay(10);


if (ir_1 == 0 || ir_2 == 0) {
    r1.write(180);
    r2.write(0);
    delay(400);
    r1.write(0);
    delay(200);
  }else{
    
    int enemyDist = US.distanceRead();
  
  //Serial.println( enemyDist);
  if(mode == 1){
      
      r1.write(0);
      r2.write(180);
      
  }else{
    if(changedir){
      r1.write(0);
      r2.write(0);
    }else{
      r1.write(180);
      r2.write(180);
    }
  }
  delay(10);
  }
}else{
  r1.write(90);
  r2.write(90);
}
  
  

  
}
