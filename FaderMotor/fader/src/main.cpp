#include <Arduino.h>

const int motorPin = 16;      // GPIO-Pin, an den der Motor (z.B. D2V5) angeschlossen ist
const int potentiometerPin = 34;  // Analoger Eingangspin, an den der Potentiometer-Abgriff (z.B. A1) angeschlossen ist

void setup() {
  Serial.begin(9600);
  
  pinMode(motorPin, OUTPUT);
  
  Serial.println("Hello Computer");
}

void loop() {
  // Lese den Wert des Potentiometers
  int potValue = analogRead(potentiometerPin);

  // Skaliere den Potentiometer-Wert auf den Bereich 0-255 (PWM-Bereich)
  int motorSpeed = map(potValue, 0, 4095, 0, 255);

  // Steuere den Motor mit der berechneten Geschwindigkeit
  digitalWrite(motorPin, motorSpeed);

  // Gib den Potentiometer-Wert aus
  Serial.println(potValue);

  delay(100);
}
