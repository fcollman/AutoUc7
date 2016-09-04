
#include "Mouse.h";

const int pinLed = LED_BUILTIN;
const int preset0AckPin = 4;
const int preset1AckPin = 5;
const int preset1pin = 3;
const int preset0pin = 2;
int presetNum = 0;
const int moveSize = 10;
const unsigned long timeOut_ = 1000;
   
void setup() {
  // Prepare led + buttons
  pinMode(pinLed, OUTPUT);
  pinMode(preset0AckPin, OUTPUT);
  pinMode(preset1AckPin, OUTPUT);
  pinMode(preset1pin, INPUT_PULLUP);
  pinMode(preset0pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(preset0pin), setPresetZero, RISING);
  attachInterrupt(digitalPinToInterrupt(preset1pin), setPresetOne, RISING);
  Serial1.begin(9600);
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
  moveHorz(x);
  moveVert(y);
}

void setPresetZero(){
  digitalWrite(preset0AckPin,LOW);
  setPreset(0);
  delay(2000);
  digitalWrite(preset0AckPin,HIGH);
}

void setPresetOne(){
  digitalWrite(preset1AckPin,LOW);
  setPreset(1);
  delay(2000);
  digitalWrite(preset1AckPin,HIGH);
}

void setPreset(int preset){
  resetPos();
  delay(2);
  moveAbsXY(5,155-13*preset);
  Mouse.click();
}

void startCutting(){
  resetPos();
}

void stopCutting(){
  resetPos();
}

bool waitForSerial(unsigned long timeOut)
{
    unsigned long startTime = millis();
    while (Serial1.available() == 0 && (millis() - startTime < timeOut) ) {}
    if (Serial1.available() > 0)
       return true;
    return false;
 }



void loop() {
  if (Serial1.available() >0){
    int inByte = Serial1.read();
    switch (inByte){
      
      //change cut thickness preset number
      case 1:
        if (waitForSerial(timeOut_)){
          int preset = Serial1.read();
          setPreset(preset);
          Serial1.write(byte(1));
          Serial1.write(preset);
        }
        break;
      
      //start cutting
      case 2:
        startCutting();
        Serial1.write(byte(2));
        break;
        
      //stop cutting
      case 3:
        stopCutting();
        Serial1.write(byte(3));
        break;

      
         
    }
  }
//  if (digitalRead(preset0pin)) {
//    digitalWrite(pinLed, HIGH);
//    setPreset(presetNum);
//    presetNum++;
//    delay(300);
//    digitalWrite(pinLed, LOW);
//    
//  }
//   if (digitalRead(preset1pin)) {
//    digitalWrite(pinLed, HIGH);
//    setPreset(presetNum);
//    presetNum++;
//    delay(300);
//    digitalWrite(pinLed, LOW);
//
//  }
//   if (digitalRead(preset2pin)) {
//    digitalWrite(pinLed, HIGH);
//    setPreset(presetNum);
//    presetNum++;
//    delay(300);
//    digitalWrite(pinLed, LOW);
//  }
//   if (presetNum>2){
//      presetNum=0;
//  }

}
