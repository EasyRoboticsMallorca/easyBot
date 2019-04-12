/* ############################################################
 *  By: Gianluca Pugliese
 *  BQlabs
 *  Android App for Controller:  https://play.google.com/store/apps/details?id=com.br3.udpctl&hl=th
 *  Hardware Micro Controller: NodeMCU V2 (ESP8266-12)

 *  Based on the awesome job of Vittaysak Rujivorakul
 *  
 *  Modified by Toniuep for easyrobotics.es
 *  Added serialCommand and Zowi standard movements. 
 *  
 *########################################################### */



#include <Servo.h>
#include <Oscillator.h>
#include <SerialCommandOld.h>
#include <EEPROM.h>
#include <Zowi.h>
//-- First step: Configure the pins where the servos are attached
/*           
 *            
 *           _______
             | o o |     <== H   
         --------------- 
 AR = SR|               |SL = AR
        |               |
        |               |
 YR ==> |               | <== YL
         --------------- 
            ||     ||
            ||     ||
            ||     ||
 RR ==>   -----   ------  <== RL
          -----   ------

*/


#define N_OSCILLATORS 9

/*
 * #define TRIM_RR -15
#define TRIM_RL 0
#define TRIM_YR -20
#define TRIM_YL -10
#define TRIM_SR -5
#define TRIM_SL -5
#define TRIM_AR  50
#define TRIM_AL -30
#define TRIM_H -20
*/

#define PIN_RR 4
#define PIN_RL 5
#define PIN_YR 6
#define PIN_YL 7
#define PIN_SR 2
#define PIN_SL 8
#define PIN_AR 3
#define PIN_AL 9
#define PIN_H 10

/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int addr = 0;
int lastCommand  = 5;
Oscillator osc[N_OSCILLATORS];

SerialCommand sCmd;


void forward(int steps=1, int T=1000);
void back(int steps=1, int T=1000);
void left(int steps=1, int T=1000);
void right(int steps=1, int T=1000);
void turnR(int steps=1, int T=1000);
void love(int steps=1, int T=700);
void swing(float steps=1, int T=1000, int h=30);
void Stop();
void punch();
void fire();
void skull();
void plus();
void mask();


void setup() {
  delay(1000);
  Serial.begin(19200);
  Serial.println("hoola Toni!");
    osc[0].attach(PIN_RR);
    osc[1].attach(PIN_RL);
    osc[2].attach(PIN_YR);
    osc[3].attach(PIN_YL);
    osc[4].attach(PIN_SR);
    osc[5].attach(PIN_SL);
    osc[6].attach(PIN_AR);
    osc[7].attach(PIN_AL);
    osc[8].attach(PIN_H);
    
    /*osc[0].SetTrim(TRIM_RR); 
    osc[1].SetTrim(TRIM_RL);
    osc[2].SetTrim(TRIM_YR);
    osc[3].SetTrim(TRIM_YL);
    osc[4].SetTrim(TRIM_SR);
    osc[5].SetTrim(TRIM_SL);
    osc[6].SetTrim(TRIM_AR);
    osc[7].SetTrim(TRIM_AL);
    osc[8].SetTrim(TRIM_H);*/

    sCmd.addCommand("T",     Trim);
    sCmd.addCommand("P",     processCommand); 
    sCmd.addDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
    Serial.println("Setting Trims from epprom");
    setStoredTrims();
    
    Serial.println("Ready");
}
void loop() {
   //checkcommand();
   //Stop(); 
    sCmd.readSerial();

       switch(lastCommand){
//UP
    
    case '1':     forward();
    break;

//DOWN
    case '2':      back();
    break;


//STOP
    case '5':  Stop();
    break;


//LEFT
    case '3':  left();
    break;


//RIGH
    case '4':  right();
    break;


//HEART
    case '6':  love();
    break;


//FIRE
    case '7':  fire();
    break;


//SKULL
    case '8':  skull();
    break;


//PLUS
    case '9':  plus();
    break;

//PUNCH
    case 'a':  swing();
    break;


//MASK
    case 'b':  mask();
    break;

    }

}

void processCommand() {
  int aNumber;
  char cmd;
  char *arg;

  Serial.println("We're in processCommand");
  arg = sCmd.next();
  if (arg != NULL) {
    //aNumber = atoi(arg);    // Converts a char string to an integer
    cmd = *arg;
    Serial.print("First argument was: ");
    //Serial.println(aNumber);
    Serial.println(cmd);
    lastCommand = cmd;
    /*switch(cmd){
//UP
    
    case '1':     forward();
    break;

//DOWN
    case '2':      back();
    break;  


//STOP
    case '5':  Stop();
    break;


//LEFT
    case '3':  left();
    break;


//RIGH
    case '4':  right();
    break;


//HEART
    case '6':  love();
    break;


//FIRE
    case '7':  fire();
    break;


//SKULL
    case '8':  skull();
    break;


//PLUS
    case '9':  plus();
    break;

//PUNCH
    case 'a':  punch();
    break;


//MASK
    case 'b':  mask();
    break;

    }*/
  }
  else {
    Serial.println("No arguments");
  }

}


void Trim(){
    int tr;
    int val;
    char *arg;
    arg = sCmd.next();
    if (arg != NULL) {
        tr = atoi(arg);
    }
    else {
      Serial.println("Falten el primer argumrnt: 0RR, 1RL, 2YR, 3YL, 4SR, 5SY, 6AR, 7AL, 8H");
    }  
  
    arg = sCmd.next();
    if (arg != NULL) {
        
        val = atoi(arg);
        osc[tr].SetTrim(val);
    
    }
    else {
      Serial.println("Falta el segon argument");
    } 
    
    addr = tr; 
    EEPROM.write(tr, val+127);
    //int r = EEPROM.read(tr);
    //Serial.println(r);
    setStoredTrims();
    Stop();
   
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized() {
  Serial.println("What?");
}

void setStoredTrims(){
    int val[9];
  for(int i = 0; i<9;i++){
      val[i] = EEPROM.read(i)-127;
    
      Serial.println(val[i]);
      osc[i].SetTrim(val[i]);
  }
}
void back(int steps, int T)
 {
 
   delay(10);


    int A[8]= {15, 15, 25, 25, 20, 20, 15, 15};
    int O[8] = {0, 0, 0, 0, -60, 60, -30, 30};
    double phase_diff[8] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90),
                            DEG2RAD(270), DEG2RAD(270), DEG2RAD(0), DEG2RAD(0)};

    for(int i=0;i<steps;i++){
      oscillate(A,O, T, phase_diff);
    }
}
 void oscillate(int A[N_OSCILLATORS], int O[N_OSCILLATORS], int T, double phase_diff[N_OSCILLATORS]){
  for (int i=0; i<8; i++) {
    osc[i].SetO(O[i]);
    osc[i].SetA(A[i]);
    osc[i].SetT(T);
    osc[i].SetPh(phase_diff[i]);
  }
  
  double ref=millis();
   for (double x=ref; x<T+ref; x=millis()){
     for (int i=0; i<8; i++){
        osc[i].refresh();
     }
  }
}



void forward(int steps, int T)
 {

   delay(10);


int A[8]= {15, 15, 25, 25, 20, 20, 15, 15};
 //int A[8]= {14, 14, 24, 24, 19, 19, 14, 14};
    int O[8] = {0, 0, 0, 0, -60, 60, -30, 30};
    double phase_diff[8] = {DEG2RAD(180), DEG2RAD(180), DEG2RAD(90), DEG2RAD(90),
                            DEG2RAD(90), DEG2RAD(90), DEG2RAD(0), DEG2RAD(0)};

    for(int i=0;i<steps;i++) {
      oscillate(A,O, T, phase_diff);
    }

 }



 void left(int steps, int T)
 {
  delay(10);


 int A[8]= {15, 15, 10, 30, 20, 20, 15, 15};
    int O[8] = {0, 0, 0, 0, -60, 60, -30, 30};
    double phase_diff[8] = {DEG2RAD(180), DEG2RAD(180), DEG2RAD(90), DEG2RAD(90),
                            DEG2RAD(90), DEG2RAD(90), DEG2RAD(0), DEG2RAD(0)};

    for(int i=0;i<steps;i++){
      oscillate(A,O, T, phase_diff);
    }

 }


 void right(int steps, int T)
 {
  delay(10);


 int A[8]= {15, 15, 30, 10, 20, 20, 15, 15};
    int O[8] = {0, 0, 0, 0, -60, 60, -30, 30};
    double phase_diff[8] = {DEG2RAD(180), DEG2RAD(180), DEG2RAD(90), DEG2RAD(90),
                            DEG2RAD(90), DEG2RAD(90), DEG2RAD(0), DEG2RAD(0)};

    for(int i=0;i<steps;i++){
    oscillate(A,O, T, phase_diff);
    }

 }

void Stop()
 {
  delay(10);


    osc[0].SetPosition(90);
    osc[1].SetPosition(90);
    osc[2].SetPosition(90);
    osc[3].SetPosition(90);
    osc[4].SetPosition(10);
    osc[5].SetPosition(170);
    osc[6].SetPosition(50);
    osc[7].SetPosition(130);
    osc[8].SetPosition(90);


 }


void love(int steps, int T)
 {
  delay(10);


    int A[8]= {22, 22, 0, 0, 0, 0, 35, 35};
    int O[8] = {25, -25, 0, 0, -60, 60, 0, 0,};
    double phase_diff[8] = {DEG2RAD(180), DEG2RAD(0), 0, 0,
                            0, 0, DEG2RAD(180), DEG2RAD(0)};

    for(int i=0;i<steps;i++){
      oscillate(A,O, T, phase_diff);
    
    }
 }
 
void swing(float steps, int T, int h){

  //-- Both feets are in phase. The offset is half the amplitude
  //-- It causes the robot to swing from side to side
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h/2, -h/2};
  double phase_diff[4] = {0, 0, DEG2RAD(0), DEG2RAD(0)};
  
  //-- Let's oscillate the servos!
  
  for(int i=0;i<steps;i++){
      oscillate(A,O, T, phase_diff);
    
    }
}


void fire()
 {
  delay(10);

    
    osc[0].SetPosition(90);
    osc[1].SetPosition(90);
    osc[2].SetPosition(90);
    osc[3].SetPosition(90);
    osc[4].SetPosition(90);
    osc[5].SetPosition(90);
    osc[6].SetPosition(40);
    osc[7].SetPosition(140);
    osc[8].SetPosition(180);
    delay(500);
    osc[8].SetPosition(0);
    delay(500);
    osc[8].SetPosition(90);
      //delay(1000);
 }


void skull()
 {
  delay(10);


    osc[0].SetPosition(40);
    osc[1].SetPosition(70);
    osc[2].SetPosition(90);
    osc[3].SetPosition(90);
    osc[4].SetPosition(10);
    osc[5].SetPosition(170);
    osc[6].SetPosition(90);
    osc[7].SetPosition(30);
 }


void plus()
 {
  delay(10);


    osc[0].SetPosition(110);
    osc[1].SetPosition(140);
    osc[2].SetPosition(90);
    osc[3].SetPosition(90);
    osc[4].SetPosition(10);
    osc[5].SetPosition(170);
    osc[6].SetPosition(150);
    osc[7].SetPosition(90);
 }


void punch()
 {
  delay(10);
 // Serial.println("punch");

/*
**************************
- PUT YOUR MOVEMENT HERE -
**************************
*/  
    //setStoredTrims();
    //Serial.println("Reading eeprom");
 }


void mask()
 {
  delay(10);

    osc[8].SetPosition(120);
    delay(1000);
    osc[8].SetPosition(90);

 }



