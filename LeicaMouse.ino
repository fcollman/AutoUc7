
#include "Mouse.h";

const int pinLed = LED_BUILTIN;
const int preset2pin = 3;
const int preset1pin = 2;
const int preset0pin = 1;
int presetNum = 0;
const int moveSize = 10;
void setup() {
  // Prepare led + buttons
  pinMode(pinLed, OUTPUT);
  pinMode(preset2pin, INPUT);
  pinMode(preset1pin, INPUT);
  pinMode(preset0pin, INPUT);
  //Serial.begin(9600);
  // Sends a clean report to the host. This is important on any Arduino type.
  Mouse.begin();
  
}

void resetPos(){
  Mouse.move(-100,-100,0);
  delay(2);
  Mouse.move(-100,-100,0);
  delay(2);
  Mouse.move(-100,-100,0);
  delay(2);
  Mouse.move(-100,-100,0);
}


void moveHorz(int pos){
   int here = 0;
  while (here<pos){
    if ((pos-here)<moveSize){
      Mouse.move(pos-here,0,0);
      here=pos;
    }
    else{
      Mouse.move(moveSize,0,0);
      here=here+moveSize;
    }
  }
}
void moveVert(int pos){

  int here = 0;
  while (here<pos){
    if ((pos-here)<moveSize){
      Mouse.move(0,pos-here,0);
      here=pos;
    }
    else{
      Mouse.move(0,moveSize,0);
      here=here+moveSize;
    }
  }
}

void moveAbsXY(int x, int y){
  resetPos();
  moveHorz(posX);
  moveVert(posY);
}

void setPreset(int preset){
  resetPos();
  delay(2);
  moveAbsXY(0,140-30*preset);
  Mouse.click();
}

void loop() {
  if (digitalRead(preset0pin)) {
    digitalWrite(pinLed, HIGH);
    setPreset(presetNum);
    delay(300);
    digitalWrite(pinLed, LOW);
    presetNum++;
  }
   if (digitalRead(preset1pin)) {
    setPreset(presetNum);
    presetNum++;

  }
   if (digitalRead(preset2pin)) {
    setPreset(presetNum);
    presetNum++;
  }
   if (presetNum>2){
      presetNum=0;
  }

}
