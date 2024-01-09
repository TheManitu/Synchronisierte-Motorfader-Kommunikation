#include <Servo.h>

Servo motorFader; // Initialisierung des Servo-Objekts für den Motorfader
int potentiometerPin = A0; // Analogeingangspin für das Schiebe-Potentiometer

void setup() {
  motorFader.attach(5); // Der Servo wird an Pin 5 des ESP32 angeschlossen
}

void loop() {
  int potValue = analogRead(potentiometerPin); // Lese den Analogwert des Potentiometers

  // Kartiere den Potentiometerwert auf den Servo-Bereich (0-180 Grad)
  int servoPosition = map(potValue, 0, 1023, 0, 180);

  // Steuere den Motorfader entsprechend der Potentiometerposition
  motorFader.write(servoPosition);

  delay(15); // Kurze Verzögerung für die Stabilität des Servo-Motors
}

//Dieser Code verwendet die Servo-Bibliothek, um den Motorfader zu steuern. Der Potentiometerwert wird eingelesen, auf den Servo-Bereich (0-180 Grad) gemappt und dann wird die Position des Motorfaders entsprechend eingestellt.