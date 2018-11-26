

float mins;
bool lastPressed;
int delayTime = 0;
int lastCount = 0;
float increment;
float incrementStep=0.5;

//pins
int buttonIncrement = 51; //pin for plus button
int buttonDecrement = 50; //pin for minus button




void setup() {
  pinMode(buttonIncrement, INPUT_PULLUP); 
  pinMode(buttonDecrement , INPUT_PULLUP);
  mins=3;
  Serial.begin(9600);
  lastPressed=false;
  
  Serial.println("Ziehzeit = " + (String) mins + " min");
}

void loop() {
  // put your main code here, to run repeatedly:
 setTime();


    
}


void setTime(){
      if (digitalRead(buttonIncrement) == LOW or  digitalRead(buttonDecrement) == LOW)
    {
      
      if (digitalRead(buttonIncrement) == LOW and  digitalRead(buttonDecrement) == HIGH) {increment = incrementStep;}
      else {if (digitalRead(buttonIncrement) == HIGH and  digitalRead(buttonDecrement) == LOW and mins > incrementStep) {increment = -incrementStep;} 
        else {increment = 0;}
      } 
    




      
      if (lastPressed != true)
      {
        lastPressed=true;
        
        mins+=increment;
        Serial.println("Ziehzeit = " + (String) mins + " min");
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
    
    delay(delayTime);
  
  }
