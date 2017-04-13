
/*
 * led matrix ir controlled
 * ir in pin 8
 * led in 12-10
 */

#include "IRremote.h"
#include "LedControl.h"

int receiver = 8; // Signal Pin of IR receiver to Arduino Digital Pin 8

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

LedControl lc=LedControl(12,10,11,1);
unsigned long delaytime1=500;
unsigned long delaytime2=50;

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

  delay(500); // Do not get immediate repeat

  if(next>64){
    next = 64;
  }

  if(next<0){
    next = 0;
  }

  return next;
} 

void draw(int val){
  int index = 0;
  lc.clearDisplay(0);
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      if(++index<val){
        lc.setLed(0,row,col,true);
      }
    }
  }
}

void setup() {

  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver

    lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);

}

int current = 32;

void loop() {

  draw(current);
  
  if (irrecv.decode(&results)){
    current = addIR(current);    
    irrecv.resume(); 
    draw(current);
  }

  delay(50);
  
}
