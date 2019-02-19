/*
  teasy bot
  v0.06

  Enrico Macholdt
  https://github.com/emacholdt/teasy_bot

*/

//////// import bibs //////// Bibliotheken importieren
#include <LiquidCrystal.h>
#include <Time.h>
#include <Servo.h>
#include <EEPROM.h>
#include "teas.h"
#include "Strings.h"

//////// version //////// Version
String vers="0.06"; 


//////// Language //////// Sprache
//////// 0 ENG //////// 1 GER
int LANG = 1; 

//////// object class for eeprom save //////// Objekt Klasse um EEPROM daten zu speichern
struct EEPROMdata {
  int LANGLast = 1;
  int inputChoiceLast = 1; // initial menu position // Anfangsposition im Menü
  int selectedTeaLast = 0; // initial tea selection // Anfangsauswahl im Menü
  float infuseMinutesLast = 3;
};


//////// create empty data object for saves //////// leeres Objekt für Speicher
EEPROMdata eeData;
int eeAddress=0;

int teaSize = sizeof(teas) / sizeof(teas[0]);

///////////////////////// for testing //////////////////////////////////// zum Testen //////////////////////////////

// temp value for battery Voltage // Platzhalterwert für die Akkuspannung
// put only the desired test value // nicht gebrauchte Werte auskommentieren mit "//"

float batteryVoltage = 2.7;        // empty // leer
// float batteryVoltage = 3.5;     // medium // mittel
// float batteryVoltage = 4.2;     // full // voll

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////  define global variables ////////  Globale Variable setzen

// mode variable // Modusvariable
int mode = 0; // initial mode // Anfangsmodus

// time variable // Zeitvariablen
long infuseTime;                // var for the infuse time in milliseconds // Variable für die Ziehzeit in Millisekunden
float infuseMinutes = 3;        // var for the infuseTime in minutes // Variable für die Ziehzeitminuten
int delayTime = 0;              // var for the waiting time // Variable für die Wartezeit
long dipTime=100;               // waiting time between dips (ms) // Wartezeit beim Eintauchen am Anfang
float increment;                // increment for time selection // Schritt bei Zeitauswahl
float incrementStep=0.25;       // increment size // Schrittgröße

//int dipDelay= 0;
//int dipCount=0;
//long infuseTime=infuseMinutes*1000*60;





// servo varibales // Variablen für den Servo
Servo myservo;            // var for the servo // Variable für den Servomotor
int maxpos = 60;          // maximal rotation // Maximale Rotation 
int dipMaxPos = 40;       // maximal rotation for initial dipping // Maximale Rotation beim Eintauchen am Anfang
int minpos = 0;           // minimal rotation // Minimale Rotation 
int cpos = maxpos;        // current position // Aktuelle Position
int pos = cpos;           // last position // Letzte Position
int stepping;             // servo speed // Servogeschwindigkeit
int dipStepping = 2;      // servo speed for dips // Servogeschwindigkeit beim Eintauchen am Anfang
int retrieveStepping = 1; // servo speed for final retrieval // Servogeschwindigkeit beim abschließenden Hochheben
int dips = 5;             // number of initial dips // Anzahl der Eintauchvorgänge am Anfang
int dipIncrement = 1;     // size of increment in dip count menu // Ansteigt von Eintauchversuchen beim Auswahlmenü





// Menu navigation helpers // Hilfsvaribalen zur Navigation im Menü
bool lastPressed; // was a button pressed in last loop // wurde in der letzten Schleife ein Taster grdrückt
int lastCount = 0; // count for loops the button was pressed // in wievielen Schleifendurchläufen wurde der Taster gedrückt
int inputChoice = 1; // initial menu position // Anfangsposition im Menü
int selectedTea = 0; // initial tea selection // Anfangsauswahl im Menü




////////  PINs ////////  PIN Belegung

// LCD 
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);
// LCD size // LCD Größe
int lcdsize[] = {16, 2};

// Buttons // Taster
int buttonIncrement = 12; // pin for plus button // Plustaster
int buttonDecrement = 11; // pin for minus button // Minustaster
int buttonNext = 10; // pin for next button // Weitertaster
int buttonBack =9; // pin for back button // Zurücktaster

// servo
int servoPin =13;





// the setup function runs once when you press reset or power the board // Setup Methode die am Anfang einmal ausgeführt wird
void setup() {
  // initialize digital pins for buttons // Initialisiere die Taster  
  pinMode(buttonIncrement, INPUT_PULLUP); 
  pinMode(buttonDecrement , INPUT_PULLUP);
  pinMode(buttonNext, INPUT_PULLUP);
  pinMode(buttonBack, INPUT_PULLUP);
  
  // initialize servo // Initialisiere den Servo 
  myservo.attach(servoPin); 
  myservo.write(cpos); // move servo to start pos // Bewege den Servo zur Startposition
  
  // load saved date
  EEPROM.get(eeAddress, eeData);
  LANG = eeData.LANGLast;
  infuseMinutes = eeData.infuseMinutesLast;
  inputChoice = eeData.inputChoiceLast;
  selectedTea = eeData.selectedTeaLast;

 
  Serial.begin(9600);
  lastPressed=false; // nothing pressed in the last loop // bisher kein Taster gedrückt

  // start LCD // starte den LCD
  lcd.begin(lcdsize[0], lcdsize[1]); 
  // print welcome message on LCD // Willkommensnachricht drucken
  lcd.print("T-easy Bot v" + vers );
  lcd.setCursor(0, 1); // go to second line // zweite Zeile
  delay(1000);
  lcd.print(welcomeString[LANG]);
  delay(1000);
  
  changeMode(1); // change the mode // Ändere den Modus
  // lcd.clear();
  // printTime();
}

// the loop function runs over and over again forever
void loop() {


   switch (mode) {
  case 1:
    indicateBattery();      // battery indicater menu // Batteriezustandsanzeige
    break;  
  case 2:
    chooseInput();          // choose to select tea or time // Wähle Teeauswahl oder Zeitauswahl
    break;    
  case 3:
    chooseTea();            // tea selection menu // Teeauswahlmenü
    break;
  case 4:
    setTime();              // time menu // Zeitauswahlmenü
    break;  
  case 5:
    setDips();              // dip menu // Eintauchmenü
    break;
  case 97:
    doStart();              // starting message // Startnachricht
    break;
  case 98:
    infuseTea();            // brew tea // Tee aufbrühen
    break;
  case 99:
    success();              // finishing message // Endnachricht
    break;
  default:
    mode = 2;
    break;
}
  

}


////////// method for changing the mode ////////// Methode zum Modusändern
void changeMode(int newMode){
  mode=newMode;     // update mode // Modus aktualisieren
  delay(500); 
  lcd.clear();      // clar the LCD // LCD löschen
}


////////// method for printing the battery status ////////// Methode zur Batterieanzeige
void indicateBattery(){  
  
  lcd.setCursor(0, 0);
  lcd.print(batteryStatusString[LANG]);
  lcd.setCursor(0, 1);
  
  if(batteryVoltage < 3.2){ lcd.print(batteryEmptyString[LANG]  + " " + (String) batteryVoltage + " V");}
  else {
    if (batteryVoltage > 3.7) { lcd.print(batteryFullString[LANG]  + " " +(String) batteryVoltage + " V");}
    else {
      lcd.print(batteryMediumString[LANG]  + " " +(String) batteryVoltage + " V");
    }
  }
  
  delay(2000);
  if (digitalRead(buttonIncrement) == LOW and  digitalRead(buttonDecrement) == LOW and  digitalRead(buttonBack) == LOW)
  {
    if(LANG==0){LANG=1;}else{LANG=0;}  
    eeData.LANGLast=LANG;
    EEPROM.put(eeAddress, eeData);
    
    lcd.clear();
    return;
  }
  changeMode(2); // change the mode // Ändere den Modus
}


////////// method for select either tea or time ////////// Methode Wähle Teeauswahl oder Zeitauswahl
void chooseInput(){
 int nextMode;
  
  if (inputChoice == 1){
    lcd.setCursor(0, 0);
    lcd.print("\176 " + chooseTeaString[LANG]);
    lcd.setCursor(0, 1);
    lcd.print("  " + chooseTimeString[LANG]);
    nextMode = 3;    
  } else {
    lcd.setCursor(0, 0);
    lcd.print("  " + chooseTeaString[LANG]);
    lcd.setCursor(0, 1);
    lcd.print("\176 " + chooseTimeString[LANG]);
    nextMode = 4;
  }
  
  if (digitalRead(buttonIncrement) == LOW or  digitalRead(buttonDecrement) == LOW)
  {
      if (lastPressed != true)
      {
        lastPressed = true;
        inputChoice *= -1;
        
        eeData.inputChoiceLast = inputChoice;
        EEPROM.put(eeAddress, eeData);
        
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
  else {// nothing pressed{
    lastPressed = false;
    delayTime=20;
    lastCount=0;
      
   }
   delay(delayTime);
  
   if(digitalRead(buttonNext) == LOW){changeMode(nextMode);return;} 
   if(digitalRead(buttonBack) == LOW){changeMode(1);return;}

}


void chooseTea(){

   if (digitalRead(buttonIncrement) == LOW or  digitalRead(buttonDecrement) == LOW)
    {
      
      if (digitalRead(buttonIncrement) == LOW and  digitalRead(buttonDecrement) == HIGH) {increment = dipIncrement;}
      else {if (digitalRead(buttonIncrement) == HIGH and  digitalRead(buttonDecrement) == LOW and dips >= dipIncrement) {increment = -dipIncrement;} 
        else {increment = 0;}
      } 
    
      if (lastPressed != true)
      {
        lastPressed=true;
        
        selectedTea+=increment;
        lcd.clear();
        if (selectedTea >= teaSize)
          {
            selectedTea = 0;
          } else {
              if (selectedTea < 0)
              {
                selectedTea = teaSize-1;
              }
          }
        eeData.selectedTeaLast = selectedTea;
        EEPROM.put(eeAddress, eeData);
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
    

  
   lcd.setCursor(0, 0);
   lcd.print(teas[selectedTea][LANG]);
   lcd.setCursor(0, 1);
   lcd.print(teas[selectedTea][2] +" " + minutesString[LANG]);
   
   if(digitalRead(buttonNext) == LOW){infuseMinutes=teas[selectedTea][2].toFloat();changeMode(97);return;} 
   if(digitalRead(buttonBack) == LOW){changeMode(1);return;}

   delay(delayTime);
}




////////// method for print time selection ////////// Methode Drucke die Zeitauswahl
void printTime()
{
  lcd.setCursor(0, 0);
  lcd.print(infuseTimeString[LANG] ); // print top row
  lcd.setCursor(0, 1);
  int intMins = (int) infuseMinutes; // calculate minutes without remainder
  int intSecs = (int) ((infuseMinutes-intMins)*60); // calculate remainder seconds
 

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



////////// method for print dip selection ////////// Methode Drucke die Eintauchauswahl
void printDips()
{
  lcd.setCursor(0, 0);
  lcd.print(numberOfDipsString[LANG] ); // print top row
  lcd.setCursor(0, 1);
  
  if(dips != 0){  // if at least one dip is selected
     lcd.print((String) dips ); // print number of dips
  }else{ // zero dips selected
      lcd.print(noneString[LANG]); // print none
  }
}





////////// method to print start message ////////// Methode Drucke die Startmitteilung
void printStart(String dot)
{
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(startingString[LANG] + " " + dot ); // print top row
  delay(1000);
}


////////// method for time selection ////////// Methode Zeitauswahl
void setTime()
{
   // if NEXT is pressed go to mode 2 
   if(digitalRead(buttonNext) == LOW){changeMode(97);return;} 
   if(digitalRead(buttonBack) == LOW){changeMode(2);return;}
   
   if (digitalRead(buttonIncrement) == LOW or  digitalRead(buttonDecrement) == LOW)
    {
      
      if (digitalRead(buttonIncrement) == LOW and  digitalRead(buttonDecrement) == HIGH) {increment = incrementStep;}
      else {if (digitalRead(buttonIncrement) == HIGH and  digitalRead(buttonDecrement) == LOW and infuseMinutes > incrementStep+0.001) {increment = -incrementStep;} 
        else {increment = 0;}
      } 
    
      if (lastPressed != true)
      {
        lastPressed=true;
        
        infuseMinutes+=increment;

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
     eeData.infuseMinutesLast=infuseMinutes;
     EEPROM.put(eeAddress, eeData);
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




////////// method for dip selection ////////// Methode Eintauchauswahl
void setDips()
{

   if(digitalRead(buttonNext) == LOW){changeMode(97);return;}
   if(digitalRead(buttonBack) == LOW){changeMode(2);return;}

   
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




////////// method for startmessage ////////// Methode Startmitteilung
void doStart()
{
   // if NEXT is pressed go to mode 3 (currently back later to 4) 
   if(digitalRead(buttonNext) == LOW){changeMode(98);return;}
   if(digitalRead(buttonBack) == LOW){changeMode(2);return;}
   String dots[4] = {"   ",".  ", ".. ","..."};
   for (int i=0;i<4;i++){
   printStart(dots[i]);
   }
   if(digitalRead(buttonBack) == LOW){changeMode(2);return;}
   mode=98;
}


////////// method for tea brewing  ////////// Methode Teebrühen
void infuseTea() {
    infuseTime=infuseMinutes*1000*60;
    for(int d = 0; d< dips; d+=1){dip(dipTime, dipMaxPos, dipStepping);};
    dip(infuseTime, maxpos, retrieveStepping);
    changeMode(99);
}


////////// method for dipping (also for infusion)  ////////// Methode zum Eintauchen (auch beim Brühen)
void dip(long delayTime, int depth, int stepping){
  for (pos = cpos; pos >= minpos; pos -= stepping) { // go down
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        cpos= pos;
        delay(15);                       // waits 15ms
    }
    int quarts=delayTime/15000;
    for(int i =0; i<quarts; i++)
    {
      //printRemaining((quarts-i)*4);
      printRemaining((delayTime-(i*15000))/60000.0);
      delay(15000);
    }
    for (pos = cpos; pos <= depth; pos += stepping) { // get up
      // in steps of 1 degree
      myservo.write(pos);              // sagt dem Servo in die Position  'pos' zu gehen
      cpos= pos;
      delay(15);                       // wartet 15ms damit der servo zeit hat zum drehen
    }
}



////////// method to print remaining infuse time ////////// Methode Drucke die verbleibende Ziehzeit
void printRemaining(float tRemaining)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(timeRemainingString[LANG] ); // print top row
  lcd.setCursor(0, 1);
  int intMins = (int) tRemaining; // calculate minutes without remainder
  int intSecs = (int) ((tRemaining-intMins)*60); // calculate remainder seconds
  

  if(intSecs != 0){  //if time with a full minute selected
    if(intMins != 0) // if less than one minute is selected
    {
      lcd.print((String) intMins + " min " + (String) intSecs + " s"); // print minutes and seconds
    } else //more than one minute is selected
    {
      lcd.print( (String) intSecs+ " s"); // print only seconds
    }
  } else // time with a minute and decimal value is selected
  {
    lcd.print((String) intMins + " min "); // print only minutes
  }
}



////////// method to print finish message ////////// Methode Drucke die Endmitteilung
void success(){
     if(digitalRead(buttonNext) == LOW){changeMode(0);return;}
      lcd.setCursor(0, 0);
      lcd.print(succesMessage1String[LANG]); // print top row
      lcd.setCursor(0, 1);
      lcd.print(succesMessage2String[LANG]);
  }
