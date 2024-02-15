#include <Arduino.h>

// Pin-Definitionen für den Motorfader
#define enA 5  // PWM-Pin für Geschwindigkeitsregelung
#define in1 9  // Richtungssteuerung 1
#define in2 8  // Richtungssteuerung 2

// Pin-Definition für den Potentiometer
const int potiPin = A1; // Pin, an dem das Potentiometer für den Motorfader angeschlossen ist

void setup() {
    // Starte die serielle Kommunikation
  Serial.begin(9600);

  // Setze die Pin-Modi
  pinMode(potiPin, INPUT);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);


}



void SlideToValue(int targetValue){
  int val = analogRead(potiPin);
  if(abs(val - targetValue) > 20){
      if(val > targetValue){
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH); 
      }else{
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW); 
      }
      analogWrite(enA, max(min(abs(val - targetValue), 255), 200));
  }else{
      // Turn off motor
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);  
      analogWrite(enA, 0);
  }
}
void readPotentiometers() {
    int potiValue = analogRead(potiPin);           
    //int motorPotiValue = analogRead(motorPotiPin); 

    // Potentiometerwerte auf der seriellen Schnittstelle anzeigen
    Serial.print("\tPotentiometerwert: ");
    Serial.println(potiValue);
    //Serial.print("\tPotentiometerwert (Motorfader): ");
    //Serial.println(motorPotiValue);
}

void loop() {
    SlideToValue(512);
     readPotentiometers(); // Funktion zum Lesen der Potentiometerwerte aufrufen
}

