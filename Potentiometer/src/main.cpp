#include <Wire.h>
#include <Arduino.h>

static const int potentiometerPin = 34;

void setup() {

  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(potentiometerPin);
  Serial.println(sensorValue);

  delay(500);
  }