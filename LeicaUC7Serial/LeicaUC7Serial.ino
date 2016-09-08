#include <Mouse.h>

/*Revision (Sept 8 2016): */
/*** Changed communication protocol to require incomig message to start with a character.
    At the moment the 'P' and 'Y' characters are handled. P stands for (Cutting Thickness) Preset,
    and should be followed by an integer x, where x = 1,2..5.
    The Y character allow you to change the deltaY movement.
    Any handled command receives a response that is formatted as: "[chx]" where ch is the
    character and x is a number, e.g. [P2] meaning that Cutting Thickness Preset 2 was executed.
    If an invalid command is received, the code returns: [Invalid Input: chx], where ch is a
    character and x is an integer.
***/

/*** Removed code for communication using digital reads and writes ***/

//Allow easy syntax for serial printing
template<class T> inline Print &operator <<(Print &o, T arg) {o.print(arg); return o;}

//moveSize is the size of the steps taken when 
//moving the mouse cursor to a particular position
const int moveSize      = 10;

//DeltaY is steps taken between presets
int       gDeltaY       = 14;

//Hardware interface
const int preset0pin    = 2;
const int preset0AckPin = 4;
const int preset1pin    = 3;
const int preset1AckPin = 5;

void setup() {
  // Prepare led + buttons
  pinMode(LED_BUILTIN,      OUTPUT);
  pinMode(preset0AckPin,    OUTPUT);
  pinMode(preset1AckPin,    OUTPUT);

  pinMode(preset0pin,       INPUT_PULLUP);
  pinMode(preset1pin,       INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(preset0pin), setPresetOne, RISING);
  attachInterrupt(digitalPinToInterrupt(preset1pin), setPresetTwo, RISING);

  //Main interface is Serial1 (Pin 0 == Rx, Pin 1 == Tx)
  Serial1.begin(9600);
  Mouse.begin();
  Serial1 <<"[LeicaUC7-Serial]";
}

void loop() {
  if(Serial1.available() > 0){
    //Handle "commands": P1, P2..P5
    //Client should end each command with a non-numeric character
    //to avoid wasted time in the parseInt function below
    char ch = Serial1.read();

    //parseInt is not so nice, returns 0 on timeout.
    int x = Serial1.parseInt();

    switch(ch){
      //Change cut thickness by preset
      case 'P': setPreset(x);            break;

      //Change deltaY movement
      case 'Y': gDeltaY = x;             break;

      default: //For debugging..
          Serial1 << "[Invalid input: "<<ch<<x<<"]\n";
      break;
    }

    //Response to client
    if(ch == 'P' || ch == 'Y')
    {
      Serial1 << "[" << ch << x << "]";
    }
  }
}

//Valid presets, 1-5 corresponds to the 'thickness' buttons
//currently found on the Leica UI. 1 -> zero thickness, 5 -> 1000 nm
void setPreset(int preset){
  if(preset > 0 && preset <= 5){
    resetPos();
    delay(2);
    moveAbsXY(5, 155 - gDeltaY*(preset - 1));
    Mouse.click();
  }
}

void setPresetOne(){
  digitalWrite(preset0AckPin,LOW);
  setPreset(1);
  delay(2000);
  digitalWrite(preset0AckPin,HIGH);
}

void setPresetTwo(){
  digitalWrite(preset1AckPin,LOW);
  setPreset(2);
  delay(2000);
  digitalWrite(preset1AckPin,HIGH);
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


