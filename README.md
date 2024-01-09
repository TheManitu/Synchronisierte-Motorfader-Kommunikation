# Interactive Pinboard - Praxiprojekt 2023/24



## Inhalt
1. [Projektübersicht](#1-projektübersicht)
2. [Benötigte Teile](#2-benötigte-teile)
3. [Vorrausetzungen](#3-vorrausetzungen)
4. [Funktionsweise](#4-funktionsweise)
   1. Architekturmodell
   2. Aktivitätsdiagram
5. [Installation](#5-installation)
6. [Ausblick / Erweiterungsmöglichkeiten](#6-ausblick--erweiterungsmöglichkeiten)


## 1. Projektmotivation

Dieses Projekt befasst sich mit der Entwicklung eines interaktiven Pinboards, das physische Interaktionen zwischen Nutzern ermöglicht. Die Forschungsfrage lautet: "Wie kann das Prototyping des interaktiven Pinboards optimal gestaltet werden, um die technische Umsetzbarkeit, Benutzerfreundlichkeit und Effektivität der physischen Interaktion zwischen den Nutzern zu gewährleisten?"

Die Motivation für dieses Vorhaben liegt im Wunsch, eine alternative Form der zwischenmenschlichen Kommunikation zu erforschen, die auf einem interaktiven Pinboard basiert. Das Projekt strebt danach, eine Balance zwischen technischer Umsetzbarkeit und Benutzerfreundlichkeit zu finden. Der Fokus liegt auf der Entwicklung eines Prototyps, der nicht nur technisch effektiv, sondern auch einfach in der Handhabung ist.

[Mehr erfahren...](Artefakte/Expose.pdf)




## 2. Benötigte Teile für den ersten Prototyp
+ 2x ESP32 Mikrocontroller oder vergleichbar
+ 1x Alps RS60N11M9 Motorfader, 5 kΩ, 0.2 W, linear
+ 1x Schiebe-Potentiometer, Stereo, 10 kΩ, linear
+ 2x Project Board (Breadboard)
+ Wire ( M/M und  M/F Jumpwire)
+ 2x Servo-Motor (Backup Aktoren)


## 3. Funktionsweise

### Architekturmodell
![alt text][Architekturmodell]


### Aktivitätsdiagram
![alt text][Aktivitätsmodell]

### Erster Code

```cpp
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
```



## 4. Vorrausetzungen
### Allgemein

## 5. Installation

## 6. Ausblick / Erweiterungsmöglichkeiten





[Aktivitätsmodell]:https://github.com/TheManitu/Interactive-Pinboard/blob/main/Artefakte/Architektur-Aktivitätsdiagramm.png
[Architekturmodell]:https://github.com/TheManitu/Interactive-Pinboard/blob/main/Artefakte/Architektur-Architekturdiagramm.png
