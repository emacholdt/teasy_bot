/*
  teasy bot
  v0.03

  Enrico Macholdt
  https://github.com/emacholdt/teasy_bot

*/

// import bibs 
#include <LiquidCrystal.h>
#include <Time.h>
#include <Servo.h>


String vers="0.03"; 


// INPUT
int dips = 4;
float infuseMinutes;

Servo myservo;  
int maxpos = 60;
int cpos = maxpos;
int pos = cpos;// Variable for temp save of the servo position 
int minpos = 0;
int steping;

long infuseTime;
//long infuseTime=infuseMinutes*1000*60;
int dipcount=0;
long diptime=100;
int delaytime;
int dipdelay= 0;
int dipmax = 40;
int dipspeed = 2;
int retrievespeed=1;






float mins; // minute variable
bool lastPressed; // was a button pressed in last loop
int delayTime = 0; //time between loops
int lastCount = 0; //count for 
float increment;
float incrementStep=0.25;
int dipIncrement = 1;

int mode = 0;

// LCD pins
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);
// LCD size
int lcdsize[] = {16, 2};

//Button pins
int buttonIncrement = 12; //pin for plus button
int buttonDecrement = 11; //pin for minus button
int buttonNext = 10; // pin for proceed button
int buttonBack =9; // pin for proceed button

//servo pin
int servoPin =13;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(buttonIncrement, INPUT_PULLUP); 
  pinMode(buttonDecrement , INPUT_PULLUP);
  pinMode(buttonNext, INPUT_PULLUP);
  pinMode(buttonBack, INPUT_PULLUP);
  
  //initialize servo
  myservo.attach(servoPin);  // deklariert den Servo auf pin 9 mit dem Servo Objekt
  myservo.write(cpos);
  
  mins=3;
  Serial.begin(9600);
  lastPressed=false;
  lcd.begin(lcdsize[0], lcdsize[1]);
  lcd.print("T-easy Bot v" + vers );
  lcd.setCursor(0, 1);
  delay(1000);
  lcd.print("Welcome");
  delay(1000);
  mode=1;
  lcd.clear();
  printTime();
  Serial.println("Ziehzeit = " + (String) mins + " min");
}

// the loop function runs over and over again forever
void loop() {


   switch (mode) {
  case 1:
    setTime();
    break;
  case 2:
    setDips();
    break;
  case 3:
    doStart();
    break;
  case 4:
    infuseTea();
    break;
  case 5:
    success();
    break;
  default:
    // statements
    break;
}
  

}

void changeMode(int newMode){mode=newMode;delay(500);lcd.clear();}

void printTime()
{
  lcd.setCursor(0, 0);
  lcd.print("infuse time" ); // print top row
  lcd.setCursor(0, 1);
  int intMins = (int) mins; // calculate minutes without remainder
  int intSecs = (int) ((mins-intMins)*60); // calculate remainder seconds

  if(intSecs != 0){  //if time with a full minute selected
    if(intMins != 0) // if less than one minute is selected
    {
      lcd.print((String) intMins + " min " + (String) intSecs+ " s"); // print minutes and seconds
    } else //more than one minute is selected
    {
      lcd.print( (String) intSecs+ " s"); // print only seconds
    }
  } else // time with a minute and decimal value is selected
  {
    lcd.print((String) intMins + " min "); // print only minutes
  }
} // end of printTime()

void printDips()
{
  lcd.setCursor(0, 0);
  lcd.print("number of dips" ); // print top row
  lcd.setCursor(0, 1);
  
  if(dips != 0){  // if at least one dip is selected
     lcd.print((String) dips ); // print number of dips
  }else{ // zero dips selected
      lcd.print("none"); // print none
  }
}

void printStart(String dot)
{
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting " + dot ); // print top row
  delay(1000);
}


void setTime()
{
   // if NEXT is pressed go to mode 2 
   if(digitalRead(buttonNext) == LOW){changeMode(2);return;} 
   if(digitalRead(buttonBack) == LOW){changeMode(1);return;}
   
   if (digitalRead(buttonIncrement) == LOW or  digitalRead(buttonDecrement) == LOW)
    {
      
      if (digitalRead(buttonIncrement) == LOW and  digitalRead(buttonDecrement) == HIGH) {increment = incrementStep;}
      else {if (digitalRead(buttonIncrement) == HIGH and  digitalRead(buttonDecrement) == LOW and mins > incrementStep+0.001) {increment = -incrementStep;} 
        else {increment = 0;}
      } 
    
      if (lastPressed != true)
      {
        lastPressed=true;
        
        mins+=increment;
        Serial.println("Ziehzeit = " + (String) mins + " min");
        lcd.clear();
      } else{
         lastCount+=1;
         lastPressed=true;
        // if (lastCount==1){delay(500);}
         if (lastCount >= 15){
           lastPressed= false;
           lastCount=0;
          }
        }
      
    } 
    else // nothing pressed
      {
        lastPressed = false;
        delayTime=20;
        lastCount=0;
      
      }
    
    printTime();
    delay(delayTime);
  
  
}


void setDips()
{
   // if NEXT is pressed go to mode 3
   if(digitalRead(buttonNext) == LOW){changeMode(3);return;}
   if(digitalRead(buttonBack) == LOW){changeMode(1);return;}

   
   if (digitalRead(buttonIncrement) == LOW or  digitalRead(buttonDecrement) == LOW)
    {
      
      if (digitalRead(buttonIncrement) == LOW and  digitalRead(buttonDecrement) == HIGH) {increment = dipIncrement;}
      else {if (digitalRead(buttonIncrement) == HIGH and  digitalRead(buttonDecrement) == LOW and dips >= dipIncrement) {increment = -dipIncrement;} 
        else {increment = 0;}
      } 
    
      if (lastPressed != true)
      {
        lastPressed=true;
        
        dips+=increment;
        Serial.println("Anzahl der Dips = " + (String) dips + " dips");
        lcd.clear();
      } else{
         lastCount+=1;
         lastPressed=true;
        // if (lastCount==1){delay(500);}
         if (lastCount >= 15){
           lastPressed= false;
           lastCount=0;
          }
        }
      
    } 
    else // nothing pressed
      {
        lastPressed = false;
        delayTime=20;
        lastCount=0;
      
      }
    
    printDips();
    delay(delayTime);
  
  
}


void doStart()
{
   // if NEXT is pressed go to mode 3 (currently back later to 4) 
   if(digitalRead(buttonNext) == LOW){changeMode(4);return;}
   if(digitalRead(buttonBack) == LOW){changeMode(2);return;}
   String dots[4] = {"   ",".  ", ".. ","..."};
   for (int i=0;i<4;i++){
   printStart(dots[i]);
   }

   changeMode(4);
}


void infuseTea() {

    infuseTime=mins*1000*60;
    for(int d = 0; d< dips; d+=1){dip(diptime, dipmax, dipspeed);};
    dip(infuseTime, maxpos, retrievespeed);

    changeMode(5);
}


void dip(long delaytime, int depth, int steping){
  for (pos = cpos; pos >= minpos; pos -= steping) { // go down
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        cpos= pos;
        delay(15);                       // waits 15ms
    }
    int quarts=delaytime/15000;
    for(int i =0; i<quarts; i++)
    {
      //printRemaining((quarts-i)*4);
      printRemaining((delaytime-(i*15000))/60000.0);
      delay(15000);
    }
    for (pos = cpos; pos <= depth; pos += steping) { // get up
      // in steps of 1 degree
      myservo.write(pos);              // sagt dem Servo in die Position  'pos' zu gehen
      cpos= pos;
      delay(15);                       // wartet 15ms damit der servo zeit hat zum drehen
    }

  
}

void printRemaining(float tRemaining)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("time remaining" ); // print top row
  lcd.setCursor(0, 1);
  int intMins = (int) tRemaining; // calculate minutes without remainder
  int intSecs = (int) ((tRemaining-intMins)*60); // calculate remainder seconds

  if(intSecs != 0){  //if time with a full minute selected
    if(intMins != 0) // if less than one minute is selected
    {
      lcd.print((String) intMins + " min " + (String) intSecs+ " s"); // print minutes and seconds
    } else //more than one minute is selected
    {
      lcd.print( (String) intSecs+ " s"); // print only seconds
    }
  } else // time with a minute and decimal value is selected
  {
    lcd.print((String) intMins + " min "); // print only minutes
  }
}

void success(){
     if(digitalRead(buttonNext) == LOW){changeMode(1);return;}
      lcd.setCursor(0, 0);
      lcd.print("Your tea is " ); // print top row
      lcd.setCursor(0, 1);
      lcd.print("ready!  Enjoy");
  }
