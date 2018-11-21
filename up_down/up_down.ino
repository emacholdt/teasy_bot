//www.elegoo.com
//2016.12.08
#include <Servo.h>


// INPUT
int dips = 4;
float infuseMinutes=0.05;


// vars
Servo myservo;  
int maxpos = 60;
int cpos = maxpos;
int pos = cpos;// Variable zum Speichern der Servoposition
int minpos = 0;
int steping;

long infuseTime=infuseMinutes*1000*60;
int dipcount=0;
long diptime=100;
int delaytime;
int dipdelay= 0;
int dipmax = 40;
int dipspeed = 2;
int retrievespeed=1;


//pins
int servopin =9;
int buttonApin = 5;
int buttonBpin = 6;
int ledPin = 7;


byte leds = 0;


void setup() {
  myservo.attach(servopin);  // deklariert den Servo auf pin 9 mit dem Servo Objekt
  pinMode(ledPin, OUTPUT);
  pinMode(buttonApin, INPUT_PULLUP);  
  pinMode(buttonBpin, INPUT_PULLUP);  
  myservo.write(cpos);
  delay(25);
}

void loop() {

  //myservo.write(pos); 
  if (digitalRead(buttonApin) == LOW)
  {
    digitalWrite(ledPin, HIGH);
    for(int d = 0; d< dips; d+=1){dip(diptime, dipmax, dipspeed);};
    dip(infuseTime, maxpos, retrievespeed);

  } else {
    digitalWrite(ledPin, LOW);
    //myservo.write(maxpos);
    if (cpos != maxpos){
      for (pos = cpos; pos <= maxpos; pos += steping) { // geht von 0 grad bis 180grad
        // in steps of 1 degree
        myservo.write(pos);              // sagt dem Servo in die Position  'pos' zu gehen
        cpos= pos;
        delay(35); 
      } 
      if (cpos != maxpos){myservo.write(maxpos);}
    }
  }
}


void dip(long delaytime, int depth, int steping){
  for (pos = cpos; pos >= minpos; pos -= steping) { // go down
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        cpos= pos;
        delay(15);                       // waits 15ms
    }
    delay(delaytime);
    for (pos = cpos; pos <= depth; pos += steping) { // get up
      // in steps of 1 degree
      myservo.write(pos);              // sagt dem Servo in die Position  'pos' zu gehen
      cpos= pos;
      delay(15);                       // wartet 15ms damit der servo zeit hat zum drehen
    }

  
}




