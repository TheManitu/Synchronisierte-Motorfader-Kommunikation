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

Herzlich willkommen zu unserem interaktiven Pinboard-Projekt! Hier dreht sich alles um die Schaffung eines innovativen Pinboards, das physische Interaktionen zwischen Nutzern ermöglicht. Die zentrale Frage, die uns antreibt, lautet: "Wie kann das Prototyping des interaktiven Pinboards optimal gestaltet werden, um die technische Umsetzbarkeit, Benutzerfreundlichkeit und Effektivität der physischen Interaktion zwischen den Nutzern zu gewährleisten?"

Unsere Motivation besteht darin, eine alternative Form der zwischenmenschlichen Kommunikation zu erforschen, die auf einem interaktiven Pinboard basiert. Wir streben danach, eine harmonische Balance zwischen technischer Raffinesse und Benutzerfreundlichkeit zu finden. Unser Hauptaugenmerk liegt auf der Entwicklung eines Prototyps, der nicht nur technisch effektiv, sondern auch einfach in der Handhabung ist. Schließ dich uns an, während wir die Grenzen der interaktiven Kommunikation erkunden und ein Pinboard gestalten, das eine neue Dimension der zwischenmenschlichen Interaktion eröffnet.

[Mehr erfahren...](Artefakte/Expose.pdf)




## 2. Benötigte Teile für den ersten Prototyp
+ 1x PI (Broker) 
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

## ESP32 Code für Potentiometer-Erfassung und -Übertragung

### Potentiometer ESP32 Code

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "DEIN_WIFI_SSID";
const char* password = "DEIN_WIFI_PASSWORT";
const char* mqtt_server = "IP_DES_MQTT_BROKERS";

WiFiClient espClient;
PubSubClient client(espClient);

const int potentiometerPin = 34;  // Analoger Eingangspin, an den der Potentiometer-Abgriff angeschlossen ist

void setup_wifi() {
  // Code für WiFi-Verbindungsaufbau hier ...
}

void reconnect() {
  // Code für MQTT-Verbindungsaufbau hier ...
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Code für Callback-Funktion hier ...
}

void setup() {
  // Setup-Funktion hier ...
}

void loop() {
  // Loop-Funktion hier ...
}
```



## ESP32 Code für MotorFader und -Übertragung

### MotorFader ESP32 Code

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "DEIN_WIFI_SSID";
const char* password = "DEIN_WIFI_PASSWORT";
const char* mqtt_server = "IP_DES_MQTT_BROKERS";

WiFiClient espClient;
PubSubClient client(espClient);

const int motorPin = 16;      // GPIO-Pin, an den der Motor angeschlossen ist

void setup_wifi() {
  // Code für WiFi-Verbindungsaufbau hier ...
}

void reconnect() {
  // Code für MQTT-Verbindungsaufbau hier ...
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Code für Callback-Funktion hier ...
}

void setup() {
  // Setup-Funktion hier ...
}

void loop() {
  // Loop-Funktion hier ...
}
```
## 4. Vorrausetzungen

### Grundinformationen zur finalen Version

Für die finale Version des Projekts müssen die Positionen von 64 Potentiometern auf 64 Potentiometern synchronisiert sein. Die beiden ESP32-Boards befinden sich voraussichtlich in zwei verschiedenen Städten, aber in einem Raum, in dem eine WLAN-Verbindung möglich ist. Die Anforderungen an die Datenübertragung sind daher, dass die synchronisierten Potentiometer-Positionen zuverlässig und in Echtzeit zwischen den beiden ESP32-Boards übertragen werden müssen.

### Übertragung der Potentiometer-Positionen

Die Auswahl von 64 Potentiometern als Anzahl der zu synchronisierenden Geräte bietet eine solide Menge an Pins für die Kommunikation zwischen den ESP32-Boards. Diese Anzahl ist aus mehreren Gründen sinnvoll:

- **Ausreichend Flexibilität:** Mit 64 Pins haben wir genügend Flexibilität, um die Kommunikation zu implementieren, ohne dass uns die Anzahl der verfügbaren Pins einschränkt. Dies ermöglicht es uns, verschiedene Kommunikationsprotokolle und -technologien zu verwenden, um die Potentiometer-Positionen zu übertragen, ohne Kompromisse bei anderen Funktionen eingehen zu müssen.

- **Nicht zu extrem:** Obwohl 64 Pins eine beträchtliche Anzahl sind, ist sie nicht zu extrem und kann immer noch gut verwaltet werden. Es ist möglich, die Pins in logische Gruppen zu organisieren und entsprechend zu verwalten, ohne dass die Komplexität außer Kontrolle gerät.

Es ist wichtig anzumerken, dass die Planung von 64 Potentiometern lediglich eine vorläufige Entscheidung ist, um die Option offen zu halten, eine große Anzahl von Potentiometern zu nutzen. Je nach den Anforderungen des endgültigen Designs können mehr oder weniger Potentiometer verwendet werden.

### Übertragung der Potentiometer-Positionen

Um sicherzustellen, dass die Positionen der Potentiometer auf dem einen ESP32 mit denen auf dem anderen synchronisiert sind, gibt es verschiedene Optionen für die Datenübertragung. Angesichts der Grundinformationen, dass die ESP32-Boards voraussichtlich in zwei verschiedenen Städten sein werden, jedoch in einem Raum mit WLAN-Verbindung, und dass insgesamt 64 Potentiometer synchronisiert werden müssen, werden im Folgenden einige dieser Optionen diskutiert:

1. **MQTT (Message Queuing Telemetry Transport):**
   - **Sinnvoll:** MQTT ist ein leichtgewichtiges Protokoll, das eine zuverlässige und schnelle Datenübertragung ermöglicht. Es bietet eine einfache Möglichkeit, Daten zwischen verschiedenen Geräten zu übertragen, und unterstützt sowohl Publish-Subscribe- als auch Punkt-zu-Punkt-Kommunikation.
   - **Nicht sinnvoll:** Möglicherweise ist eine zusätzliche Infrastruktur wie ein MQTT-Broker erforderlich, was zusätzlichen Aufwand für die Einrichtung und Wartung bedeutet. Die Verwendung von MQTT erfordert jedoch keine direkte Verbindung zwischen den ESP32-Boards und bietet eine flexible Möglichkeit, die Potentiometer-Positionen zu übertragen.

2. **Direkte drahtlose Kommunikation (z. B. Wi-Fi Direct):**
   - **Sinnvoll:** Direkte drahtlose Kommunikation ermöglicht eine einfache Punkt-zu-Punkt-Verbindung zwischen den beiden ESP32-Geräten, ohne dass zusätzliche Infrastruktur erforderlich ist. Dies könnte eine gute Option sein, wenn eine schnelle und zuverlässige Verbindung zwischen den Geräten erforderlich ist.
   - **Nicht sinnvoll:** Die Reichweite von Wi-Fi Direct kann begrenzt sein, insbesondere in Umgebungen mit vielen Störquellen. Dies könnte problematisch sein, wenn sich die ESP32-Boards in verschiedenen Städten befinden.

3. **Bluetooth:**
   - **Sinnvoll:** Bluetooth bietet eine einfache Möglichkeit zur drahtlosen Datenübertragung zwischen zwei Geräten und ist weit verbreitet. Es ist eine gute Option für kurze bis mittlere Entfernungen und bietet eine einfache Einrichtung.
   - **Nicht sinnvoll:** Möglicherweise ist eine zusätzliche Konfiguration erforderlich, und die Übertragungsgeschwindigkeit könnte langsamer sein als bei anderen Optionen. Zudem könnte die Reichweite von Bluetooth begrenzt sein, was bei der Übertragung zwischen verschiedenen Städten problematisch sein könnte.

4. **LoRaWAN (Long Range Wide Area Network):**
   - **Sinnvoll:** LoRaWAN bietet eine große Reichweite und eignet sich daher gut für die drahtlose Kommunikation über große Entfernungen. Es ist eine gute Option, wenn die ESP32-Geräte weit voneinander entfernt sind und eine stabile Verbindung benötigen.
   - **Nicht sinnvoll:** Die Einrichtung eines LoRaWAN-Netzwerks erfordert möglicherweise zusätzliche Hardware und Infrastruktur, was zusätzlichen Aufwand bedeuten kann. Zudem könnte die Übertragungsgeschwindigkeit langsamer sein als bei anderen Optionen, was für Echtzeit-Anwendungen wie die Synchronisation von Potentiometern problematisch sein könnte.

### Fazit

Die Auswahl der geeigneten Methode zur Übertragung der Potentiometer-Positionen hängt von verschiedenen Faktoren ab, einschließlich der Anforderungen an Reichweite, Zuverlässigkeit, Komplexität und verfügbarer Infrastruktur. Angesichts der gegebenen Grundinformationen könnte eine WLAN-Verbindung eine sinnvolle Wahl sein, um die Implementierung eines zusätzlichen Brokers zu vermeiden, auch wenn MQTT eine gute Option wäre. Die anderen Optionen fallen aufgrund ihrer negativen Punkte aus, insbesondere in Bezug auf Reichweite und Konfigurationsaufwand.



## 5. Installation

## 6. Ausblick / Erweiterungsmöglichkeiten





[Aktivitätsmodell]:https://github.com/TheManitu/Interactive-Pinboard/blob/main/Artefakte/Architektur-Aktivitätsdiagramm.png
[Architekturmodell]:https://github.com/TheManitu/Interactive-Pinboard/blob/main/Artefakte/Architektur-Architekturdiagramm.png
