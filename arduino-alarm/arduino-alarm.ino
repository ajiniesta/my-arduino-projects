
/*
  Alarm....
  keypad in pins: 53-41 (7 pins in total)
  -----
  Digital inputs
    Next 2
    Prev 3
  Pir pin: 21
  lcd: 36, 34, 32, 30, 28, 26
  armed led: 6
  disarmed led: 5
*/
#include <Keypad.h>
#include <SimpleDHT.h>
#include <LiquidCrystal.h>

//Keypad Section
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {53, 51, 49, 47}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {45, 43, 41}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

char getKey() {
  return customKeypad.getKey();
}
//End keypad section

//dht11 section
int pinDHT11 = 4;
SimpleDHT11 dht11;

//end of dht11 section

//section of the lcd
LiquidCrystal lcd(36, 34, 32, 30, 28, 26);
//end of lcd

//Section for PIR sensor
int pirPin = 21; // To be checked via ISR
int ledPin = 7;
int pirValue;
boolean armed = false;
int armedPin = 6;
int disarmedPin = 5;
int detections = 0;

void detected() {
  if (armed) {
    pirValue = 1;
    Serial.println("Alarm Detection!!!");
    digitalWrite(ledPin, HIGH);
    detections++;
  } else {
    Serial.println("Detection but disarmed!!!");
  }
  attachInterrupt(digitalPinToInterrupt(pirPin), undetected, FALLING);
}

void undetected() {
  pirValue = 0;
  Serial.println("Alarm going off!!!");
  digitalWrite(ledPin, LOW);
  attachInterrupt(digitalPinToInterrupt(pirPin), detected, HIGH);
}
//end of pir sensor section

//Functions of the states
void funAlarm() {
  //Serial.println("In alarm mode....");
  //delay(400);
  lcd.clear();
  lcd.home();
  lcd.print("Alarm ");
  lcd.print(armed ? "Armed" : "Disarmed");
  lcd.setCursor(0, 1);
  lcd.print("# detections: ");
  lcd.print(detections);
  delay(250);
}

void funTemp() {
  Serial.println("=================================");
  Serial.println("Sample DHT11...");

  // read with raw sample data.
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    Serial.print("Read DHT11 failed");
    return;
  }

  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" %");

  lcd.clear();
  lcd.home();
  lcd.print("Temp: ");
  lcd.print((int)temperature);
  lcd.print(" *C");
  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  lcd.print((int)humidity);
  lcd.print(" %");
  // DHT11 sampling rate is 1HZ.
  delay(1000);
}

void funLock() {

  lcd.clear();
  lcd.home();
  lcd.print("*/#/0 to a/d/s");
  lcd.setCursor(0, 1);
  lcd.cursor();

  char key = getKey();
  if (key && key == '*') {
    armed = true;
    lcd.print("Armed");
    armLeds();
    delay(2000);
  } else if (key && key == '#') {
    armed = false;
    lcd.print("Disarmed");
    armLeds();
    delay(2000);
  } else if (key && key == '0'){
    armed = false;
    armLeds();
    undetected();
    lcd.print("Stopping alarm");
    delay(2000);
  }
  delay(250);
}

void armLeds() {
  if (armed) {
    digitalWrite(armedPin, HIGH);
    digitalWrite(disarmedPin, LOW);
  } else {
    digitalWrite(armedPin, LOW);
    digitalWrite(disarmedPin, HIGH);
  }
}
//End of functions of the states


//Digital Input and states section
int buttonNextPin = 2;
int buttonPrevPin = 3;

typedef void (* behaviour) ();

typedef struct {
  const char* code;
  const char* desc;
  behaviour fun;
} AAState;
int stateIndex = 0;
const AAState alarmState = {"ALARM", "Alarm", funAlarm};
const AAState tempState = {"TEMP", "Temperature", funTemp};
const AAState lockState = {"LOCK", "Locking the alarm", funLock};
const AAState states[] = {alarmState, tempState, lockState};
int statesLen = 3;
unsigned long auxMillis = 0;

int nextStateIndex() {
  int newIndex = stateIndex;
  if (digitalRead(buttonNextPin) == LOW) {
    newIndex++;
  } else if (digitalRead(buttonPrevPin) == LOW) {
    newIndex--;
  }
  if (newIndex == statesLen) {
    newIndex = 0;
  }
  if (newIndex < 0) {
    newIndex = statesLen - 1;
  }
  return newIndex;
}

void printCurrentState() {
  Serial.print("Current state is ");
  Serial.println(states[stateIndex].code);
}

/*
  boolean rangeIsOk() {
  unsigned long current = millis();
  return (current - auxMillis) > 500;
  }
  void transition() {
  int newIndex = nextStateIndex();
  if (rangeIsOk() && newIndex != stateIndex) {
    printCurrentState();
    Serial.println("Transition");
    stateIndex = newIndex;
    printCurrentState();
    auxMillis = millis();
  }
  }*/

void nextTransition() {

  Serial.println(">>>>>>>Next transition");
  stateIndex++;
  if (stateIndex == statesLen) {
    stateIndex = 0;
  }
  printCurrentState();

}

void prevTransition() {

  Serial.println(">>>>>>>>>>>Previous transition");
  stateIndex++;
  if (stateIndex < 0) {
    stateIndex = statesLen - 1;
  }
  printCurrentState();

}


//End digital input section

void setup() {
  pinMode(buttonNextPin, INPUT_PULLUP);
  pinMode(buttonPrevPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(armedPin, OUTPUT);
  pinMode(disarmedPin, OUTPUT);
  armLeds();
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  while (!Serial);
  lcd.begin(16, 2);
  lcd.print("hola, antonio!");
  printCurrentState();
  attachInterrupt(digitalPinToInterrupt(buttonNextPin), nextTransition, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPrevPin), prevTransition, FALLING);
  attachInterrupt(digitalPinToInterrupt(pirPin), detected, HIGH);
}

void loop() {

  states[stateIndex].fun();

}
