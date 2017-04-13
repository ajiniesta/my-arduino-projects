#include <toneAC.h>

/*
 * Theremin... with arduino
 * Proximity sensor and a passive buzzer
*/
//#include "IRremote.h"
#include "pitches.h"
#include "SR04.h"


int pin_trig=12;
int pin_echo=11;

int pin_buzz=10;

int pin_ir=9;

SR04 sr04 = SR04(pin_echo,pin_trig);
long a;
/*
IRrecv irrecv(pin_ir);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

boolean add = true;

int addIR(int previous){

  int next = previous;

  switch(results.value)

  {
  case 0xFF629D: 
    next++; 
    add = true;
    Serial.println("more leds");
    break;
  case 0xFFA857: 
    next--;
    add = false;
    Serial.println("less leds");
    break;
  case 0xFFFFFFFF: 
    Serial.println(" REPEAT");
    if(add)
      next++;
     else
      next--;
    break;  
  default: 
    Serial.println(" other button   ");

  }// End Case

  delay(50); // Do not get immediate repeat

  if(next>64){
    next = 64;
  }

  if(next<0){
    next = 0;
  }

  return next;
} 
*/
// notes in the melody:
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int del = 50;  // 500 miliseconds
int duration = 250;
int mult = 4;

void setup() {
  Serial.begin(9600);
  delay(1000);
  //irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  a=sr04.Distance();
  int note = getNote(a*mult);
  Serial.print(a);
  Serial.print(" -- Note freq: ");
  Serial.print(note);
  Serial.print(" Multiplier: ");
  Serial.print(mult);
  Serial.print(" --duration note: ");
  Serial.println(duration);

  tone(pin_buzz, note, duration);
  
  delay(del);

//  freq = addIR(freq);
}

int getNote(int input){
  char* notes[] = {"NOTE_B0"  ,   "NOTE_C1"  ,   "NOTE_CS1" ,   "NOTE_D1"  ,   "NOTE_DS1" ,   "NOTE_E1"  ,   "NOTE_F1"  ,  "NOTE_FS1" ,   "NOTE_G1"  ,   "NOTE_GS1" ,   "NOTE_A1"  ,   "NOTE_AS1" ,   "NOTE_B1"  ,   "NOTE_C2"  ,   "NOTE_CS2" ,   "NOTE_D2"  ,   "NOTE_DS2" ,   "NOTE_E2"  ,   "NOTE_F2"  ,   "NOTE_FS2" ,   "NOTE_G2"  ,   "NOTE_GS2" ,   "NOTE_A2"  ,   "NOTE_AS2" ,   "NOTE_B2"  ,   "NOTE_C3"  ,   "NOTE_CS3" ,   "NOTE_D3"  ,   "NOTE_DS3" ,   "NOTE_E3"  ,   "NOTE_F3"  ,   "NOTE_FS3" ,   "NOTE_G3"  ,   "NOTE_GS3" ,   "NOTE_A3"  ,   "NOTE_AS3" ,   "NOTE_B3"  ,   "NOTE_C4"  ,   "NOTE_CS4" ,   "NOTE_D4"  ,   "NOTE_DS4" ,   "NOTE_E4"  ,   "NOTE_F4"  ,   "NOTE_FS4" ,   "NOTE_G4"  ,   "NOTE_GS4" ,   "NOTE_A4"  ,   "NOTE_AS4" ,   "NOTE_B4"  ,   "NOTE_C5"  ,   "NOTE_CS5" ,   "NOTE_D5"  ,   "NOTE_DS5" ,   "NOTE_E5"  ,   "NOTE_F5"  ,   "NOTE_FS5" ,   "NOTE_G5"  ,   "NOTE_GS5" ,   "NOTE_A5"  ,   "NOTE_AS5" ,   "NOTE_B5"  ,   "NOTE_C6"  ,   "NOTE_CS6" ,   "NOTE_D6"  ,   "NOTE_DS6" ,   "NOTE_E6"  ,   "NOTE_F6"  ,   "NOTE_FS6" ,   "NOTE_G6"  ,   "NOTE_GS6" ,   "NOTE_A6"  ,   "NOTE_AS6" ,   "NOTE_B6"  ,   "NOTE_C7"  ,   "NOTE_CS7" ,   "NOTE_D7"  ,   "NOTE_DS7" ,   "NOTE_E7"  ,   "NOTE_F7"  ,   "NOTE_FS7" ,   "NOTE_G7"  ,   "NOTE_GS7" ,   "NOTE_A7"  ,   "NOTE_AS7" ,   "NOTE_B7"  ,   "NOTE_C8"  ,   "NOTE_CS8" ,   "NOTE_D8"  ,   "NOTE_DS8"};
  int rangeNotes[] = {31,33,35,37,39, 41, 44, 46, 49, 52, 55, 58, 62, 65,69,73,78,82, 87,93,98,104, 110, 117,123,131,139, 147, 156,  165,  175, 185,  196, 208,  220, 233,  247, 262,277, 294,311, 330, 349,370, 392,415, 440,466, 494, 523,554, 587,622, 659, 698,740, 784,831, 880,932, 988, 1047,1109, 1175,1245, 1319, 1397,1480, 1568,1661, 1760,1865, 1976, 2093,2217, 2349,2489, 2637, 2794,2960, 3136,3322, 3520,3729, 3951, 4186,4435, 4699,4978};
  for(int i=0; i<89; i++){
    if(input<rangeNotes[i]){
      Serial.print("Note selected: ");
      Serial.print(notes[i]);
      Serial.print("-");
      return rangeNotes[i];
    }
  }
  return rangeNotes[0];
}



