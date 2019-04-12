#include <Servo.h>
#include <Ultrasonic.h>
#include <printf.h>

const int r1Pin = 4;
const int r2Pin = 5;

const int echoPin = 7;
const int trigPin = 6;

const int ir1Pin = 2;
const int ir2Pin = 3;
const int pin_bt_red = 8;
const int pin_bt_white = 9;
const int pin_bt_blue = 10;

Servo r1, r2;
Ultrasonic US(trigPin, echoPin);

boolean enemyDetected = false;
boolean changedir = false;
int mode = 1;
int lastmode = 0;
boolean sumoMode = true;
boolean go = false;
int searchMode = 0;
int index = 1;

unsigned long previousMillis = 0;

// constants won't change :
const long interval = 400;
int t = 80;

void setup()
{
  while (!Serial);
  Serial.begin(115200);
  pinMode(pin_bt_red, INPUT_PULLUP);
  pinMode(pin_bt_white, INPUT_PULLUP);
  pinMode(pin_bt_blue, INPUT_PULLUP);

  r1.attach(r1Pin);
  r2.attach(r2Pin);
  pinMode(ir1Pin, INPUT);
  pinMode(ir2Pin, INPUT);
  r1.write(90);
  r2.write(90);
  Serial.print(sumoMode);
  delay(4000);
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
   // if (mode != 0) {
      if (index < 5)
        index++;
      else
        index = 1;
    //}
    previousMillis = currentMillis;
  }
  //Serial.println(scanResult);

  delay(10);
  int bt_red = digitalRead(pin_bt_red);
  int bt_white = digitalRead(pin_bt_white);
  int bt_blue = digitalRead(pin_bt_blue);

  if ((bt_red == LOW || bt_white == LOW || bt_blue == LOW) && !go) {
    sumoMode = true;
    go = true;
    if (bt_red == LOW) {
      searchMode = 1;
    } else if (bt_white == LOW) {
      searchMode = 2;
    } else if (bt_blue == LOW) {
      searchMode = 3;
    }
    delay(3000);

  }

  if (sumoMode) {
    int ir_1 = digitalRead(ir1Pin);
    int ir_2 = digitalRead(ir2Pin);

    int scanResult = US.distanceRead();
    delay(10);

    if (scanResult != 0 && scanResult < 30) {
      mode = 1;
    } else {
      mode = 0;
    }

    if (lastmode != mode) {
      if (mode == 0) {
        Serial.println("Atack mode ON!!");
      }
      if (mode == 1) {
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
    } else {

      int enemyDist = US.distanceRead();

      Serial.println( enemyDist);
      if (mode == 1) {

        r1.write(0);
        r2.write(180);
        //delay(100);

      } else {
        //SearchMode(searchMode, index);
       if (changedir) {
          r1.write(0+t);
          r2.write(0+t);
        } else {
          r1.write(180-t);
          r2.write(180-t);
        }
      }
      delay(10);
    }
  }
}

void MoveForward() {
  r1.write(0+t);
  r2.write(180-t);

}
void MoveBack() {
  r1.write(180-t);
  r2.write(0+t);

}
void TurnRight() {
  r1.write(90);
  r2.write(180);

}
void TurnLeft() {
  r1.write(0);
  r2.write(90);

}
void SpinRight() {
  r1.write(0+t);
  r2.write(0+t);

}
void SpinLeft() {
  r1.write(180-t);
  r2.write(180-t);
 
}
void _stop() {
  r1.write(90);
  r2.write(90);

}

void goA(int index) {
  //up,right,up,right, spinLeft;
  switch (index) {
    case 1:
      MoveForward();
      break;
    case 2:
      TurnRight();
      break;
    case 3:
      MoveForward();
      break;
    case 4:
      TurnRight();
      break;
    case 5:
      SpinLeft();
      break;

  }
}
  void goB(int index) {
    //right,up,spinRight,Left,down
    switch (index) {
      case 1:
        TurnRight();
        break;
      case 2:
        MoveForward();
        break;
      case 3:
        SpinRight();
        break;
      case 4:
        TurnLeft();
        break;
      case 5:
        MoveBack();
        break;
    }
  }
  void goC(int index) {
    //right,up,spinLeft,up,rigth
    switch (index) {
      case 1:
        TurnRight();
        break;
      case 2:
        MoveForward();
        break;
      case 3:
        SpinLeft();
        break;
      case 4:
        MoveForward();
        break;
      case 5:
        TurnRight();
        break;
    }
  }
  void SearchMode(int type, int index) {
    if (type == 1) {
      goA(index);
    } else if (type == 2) {
      goB(index);
    } else if (type == 3) {
      goC(index);
    }
  }
