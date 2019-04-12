//#include <SerialCommandOld.h>

#include "pitches.h"

const int ENA = 6; // ENA eh o pino de controle de velocidade no motor
const int IN1 = 4; //IN1 motor = pino 6 no arduino
const int IN2 = 5;

const int IN3 = 8; //IN3 motor = pino 10 no arduino
const int IN4 = 7;
const int ENB = 9;

const int IR_DRETA =  2, IR_DARRERA = 11, IR_ESQ = 3 ;
const int START = 10;

const int LEDS = 13;

bool lluita = false;

int tiempo = 0;
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long interval = 10;           // interval at which to blink (milliseconds)

//SerialCommand sCmd;
bool st = false;

bool S[2];

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
void setup() {
  Serial.begin(38400); //Inicializa comunicacio Serial
  pinMode(LEDS, OUTPUT);
  Serial.println("SumoBot: Holaa !!...");

  //pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  //pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IR_DRETA, INPUT);
  pinMode(IR_DARRERA, INPUT);
  pinMode(IR_ESQ, INPUT);

  pinMode(START, INPUT_PULLUP);

  digitalWrite(LEDS, LOW);
  analogWrite(ENA, 127);
  analogWrite(ENB, 127);

  /* sCmd.addCommand("f", forward);
    sCmd.addCommand("s", Stop);
    sCmd.addCommand("r", right);
    sCmd.addCommand("l", left);
    sCmd.addCommand("b", back);

    sCmd.addDefaultHandler(unrecognized);
  */
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(12, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
  Serial.println("EasySUMO Peparat...");

}

void loop() {
  unsigned long currentMillis = millis();

  //Serial.println(digitalRead(START));
  if (lluita == true) {
    digitalWrite(LEDS, HIGH);




    if (!digitalRead(IR_DRETA)) {
      analogWrite(ENA, 200);
      analogWrite(ENB, 200);
      back();
      delay(300);
      analogWrite(ENA, 160);
      analogWrite(ENB, 160);
      left();
      
      delay(200);


    }else if (!digitalRead(IR_ESQ)) {
      analogWrite(ENA, 200);
      analogWrite(ENB, 200);
      back();
      delay(300);
      analogWrite(ENA, 160);
      analogWrite(ENB, 160);
      right();
      
      delay(200);


    }


    //Serial.println("_____");
    //delay(500);

    cercarObjectiu();

  } else if (digitalRead(START) == 0) {


    lluita = true;
    delay(200);
    tone(12, NOTE_C5, 200);
    delay(800);
    tone(12, NOTE_C5, 200);
    delay(800);
    tone(12, NOTE_G5, 800);
    delay(1000);
    // stop the tone playing:
    noTone(12);


    Serial.println("gooooooo");
  }

}




/////------------------ACABA EL LOOP();
/*


   //DEFINICIÓ DE FUNCIONS
*/

int cercarObjectiu() {

  // LLetgim el promitg de les entrades A0 i A1 :
  int ADC_SHARP0 = ADC_promitg0(100);


  if (ADC_SHARP0 > 190) {
    Serial.print("Objectiu Detectat...");
    Serial.print("IR_DRETA ");
    Serial.println(ADC_SHARP0);
    S[0] = true;
  }
  else
  {
    S[0] = false;
  }
  int ADC_SHARP1 = ADC_promitg1(100);
  if (ADC_SHARP1 > 190) {
    Serial.print("Objectiu Detectat...");
    Serial.print("IR_ESQ ");
    Serial.println(ADC_SHARP1);
    S[1] = true;
  }
  else
  {
    S[1] = false;
  }

  Serial.print(S[0]);
  Serial.print(" , ");
  Serial.println(S[1]);
  if ((S[0] != 0) || (S[1] != 0))
  {
     forward();
    analogWrite(ENA, 255);
    analogWrite(ENB, 255);
  } else {
   
    analogWrite(ENA, 127);
    analogWrite(ENB, 127);
    right();
  }


}

int ADC_promitg0(int n)
{
  long suma = 0;
  for (int i = 0; i < n; i++)
  {
    suma = suma + analogRead(A0);
  }
  return (suma / n);
}

int ADC_promitg1(int n)
{
  long suma = 0;
  for (int i = 0; i < n; i++)
  {
    suma = suma + analogRead(A1);
  }
  return (suma / n);
}



void left() {
  Serial.println("left");

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

}

void right() {

  Serial.println("right");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

}

void forward() {

  Serial.println("forward");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);


}
void back() {


  Serial.println("back");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);



}
void Stop() {
  Serial.println("Stop");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}






