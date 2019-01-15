//www.elegoo.com
//2016.12.08
#include <Servo.h>

Servo myservo;  // Erstellen Sie objekt vom typ Servo, um ein Servo zu steuern   -- 
// sollte dies unklar sein was ein Objekt sein soll, 
//bitte etwas schlau lesen zum Thema Objektorientierte Programmierung
// Zwölf Servoobjekte können auf den meisten Boards erstellt werden

int pos = 25;    // Variable zum Speichern der Servoposition
int pin = 9;

void setup() {
  myservo.attach(pin);  // deklariert den Servo auf pin 9 mit dem Servo Objekt
  myservo.write(pos); 
}

void loop() {

}
