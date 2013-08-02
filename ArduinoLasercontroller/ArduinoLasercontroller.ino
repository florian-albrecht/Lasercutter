/*
Arduino as a Controller for a Lasercutter made out of 2 Scanner

Works together with another Programm called Lasersnap
 

 by Florian Albrecht
 
 */

#include <Stepper.h>

#define steppspermmx 1  //how many steps does axis x need for one mm?
#define speppspermmy 1  //how many steps does axis y need for one mm?

#define Laserpin 2 //The Pin on which the Relaise for the Laser is attached


#define motorSteps 200     // change this depending on the number of steps
#define motorStepsz 200

// initialize of the Stepper library:
Stepper mySteppery(100, 9,10,11,12); 
Stepper myStepperx(200, 4,5,6,7);
void setup() {
  // set the motor speed
  myStepperx.setSpeed(50);
  mySteppery.setSpeed(20);
  // Initialize the Serial port:
  Serial.begin(9600);

  // set up the LED pin:
  //pinMode(ledPin, OUTPUT);
  // blink the LED:
  pinMode(Laserpin, OUTPUT);
  blink(3);
}

int nowx = 0, nowy = 0; //position of Laser


float xyratio = 1.4; //ratio of gears of y and x
int t = 1;
double get = 0;


void loop() {
  digitalWrite(Laserpin, HIGH);
  if(Serial.available()>0){
    int coordx, coordy; //receiving coordinates
    boolean statuss;
    int stat = Serial.read();
    if(stat == 49){ //if first character ist 1 Laser will be turned on
      statuss = true;
    }
      else{
        statuss = false;
    }
    delay(40);
    Serial.read();
    coordx = getint();
    Serial.print("  x:");
    Serial.print(coordx);
    delay(20);
    //Serial.read();
    coordy = getint();
    Serial.print("  y:");
    Serial.println(coordy);
    Serial.println("");
    movelaser(coordx, coordy, statuss);
    statuss = false;
  }
}

void movelaser(int x, int y, boolean ison){
  int movex, movey;
  movex = (x - nowx)*0.1*steppspermmx;
  movey = (y - nowy)*0.1*steppspermmy;
  int directionx, directiony;
  if(ison){
    digitalWrite(Laserpin, LOW);
  }
  if(movex > 0){
    directionx = 1;
   }
   else {
    directionx = -1;
    movex *= -1;
  }
  if(movey > 0){
    directiony = 1;
    }
    else{
      directiony = -1;
      movey *= -1;
    }
  int steppedx = 0;
  int steppedy = 0;
  if(movex > movey){
    myStepperx.step(directionx);
    steppedx++;
    }
    else{
      mySteppery.step(directiony);
      steppedy++;
  }
  /*Serial.println(steppedx);
  Serial.println(movex);
  Serial.println(steppedy);
  Serial.println(movey);*/
  float dx, dy;
  while(steppedx < movex || steppedy < movey){
    dx = (float(steppedx))/(float(movex));
    dy = (float(steppedy))/(float(movey));
    if(dx < dy){
      myStepperx.step(directionx);
      steppedx++;
    }
    else{
      mySteppery.step(directiony);
      steppedy++;
     }
  }
  nowx = x;
  nowy = y;
  digitalWrite(Laserpin, HIGH);
  Serial.print("d");
}

// Blink the reset LED:
void blink(int howManyTimes) {
  int i;
  for (i=0; i< howManyTimes; i++) {
    //digitalWrite(ledPin, HIGH);
    delay(200);
    //digitalWrite(ledPin, LOW);
    delay(200);
  }
}


int getint(){
  boolean minus = false;
  boolean stopper = true;
  int l = 1;
  int number = 0;
  int get[20];
  int i = 0;
  while(stopper){
    get[i] = Serial.read() - 48;
    if(get[i] < 11){
      if(get[i] == -3){
        minus = true;
      }
      else{
        if(get[i] < 10){
          i++;
        }
      }
    }
    else{
        stopper = false;
     }
      delay(30);
    }
    i--;
    if(i > 0){
      int a = i;
      while(a > 0){
        l *= 10;
        a--;
      }
    }      
    int c = 1;
    while(i>=0){
      number = number + get[i]*c;
      c *= 10;
      i--;
    }
  if(minus){
    number *= -1;
  }
  return number;
}
