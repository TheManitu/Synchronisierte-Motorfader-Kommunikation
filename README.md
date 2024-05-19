

# Interactive Pinboard - Praxisprojekt 2023/24



## Inhalt
1. [Projektübersicht](#1-projektübersicht)
2. [Benötigte Teile](#2-benötigte-teile)
3. [Funktionsweise](#3-funktionsweise)
   1. Architekturmodell
   2. Programmlogik
4. [Vorrausetzungen](#4-Vorrausetzungen)
5. [Installation](#5-installation)
6. [Ausblick / Erweiterungsmöglichkeiten](#6-ausblick--erweiterungsmöglichkeiten)


## 1. Projektmotivation

### Synchronisierter Motorfader Projekt
Willkommen zu meinem synchronisierten Motorfader Projekt! Diese Anwendung ermöglicht eine Form der physischen Kommunikation unter Freunden. 





## 2. Benötigte Teile für den ersten Prototyp
+ 2x ESP32 Mikrocontroller oder vergleichbar
+ 2x Alps RS60N11M9 Motorfader, 5 kΩ, 0.2 W, linear
+ 2x Project Board (Breadboard)
+ Wire ( M/M und  M/F Jumpwire)
+ 2x 9-12V AC Strom Netzteil
+ 2x L298N H-Bridge

+ ### Skizze
![alt text][Skizze]


## 3. Funktionsweise

### Architekturmodell
![alt text][Architekturmodell]


### Programmlogik
![alt text][Logik]

###  Code

## WiFi- und MQTT-Verbindung

- **WiFi-Verbindung:** Konfiguriert die Verbindung zu Ihrem WLAN-Netzwerk mithilfe von SSID und Passwort.
- **MQTT-Verbindung:** Konfiguriert die Verbindung zum MQTT-Broker mithilfe von Broker-Adresse, Port, Benutzername und Passwort. Der Client wird auch konfiguriert, um auf eingehende Nachrichten zu reagieren.

## Setup und Initialisierung

- **setupWifi():** Verbindet den ESP32 mit dem WLAN-Netzwerk und gibt die IP-Adresse des ESP32 über die serielle Schnittstelle aus.
- **reconnect():** Stellt die Verbindung zum MQTT-Broker wieder her, falls die Verbindung unterbrochen wird.
- **setup():** Initialisiert die Pins für Potentiometer, Motoren und WiFi, richtet den MQTT-Client ein und startet die serielle Kommunikation.

## Funktionen zur Motorsteuerung

- **movetoPosition():** Bewegt den Motor auf eine bestimmte Zielposition basierend auf den empfangenen MQTT-Nachrichten.
- **publishPotiValue():** Veröffentlicht den aktuellen Potentiometerwert über MQTT.
- **sendCurrentPosition():** Überwacht Änderungen der Potentiometerposition und veröffentlicht diese, wenn sich der Wert signifikant ändert.

## MQTT-Callback und Nachrichtenverarbeitung

- **callback():** Verarbeitet eingehende MQTT-Nachrichten und aktualisiert die globalen Variablen mit den empfangenen Daten.
- **positionChanged():** Überprüft Änderungen der Potentiometerposition und aktualisiert den Status der Bewegung.

## Hauptprogrammschleife

- **loop():** Überprüft die Verbindung zum MQTT-Broker, führt die Motorsteuerungsfunktionen aus und behandelt eingehende MQTT-Nachrichten.


## 4. Vorrausetzungen

### Übertragung der Potentiometer-Positionen

Um sicherzustellen, dass die Positionen der Potentiometer auf dem einen ESP32 mit denen auf dem anderen synchronisiert sind, gibt es verschiedene Optionen für die Datenübertragung. Angesichts der Grundinformationen, dass die ESP32-Boards voraussichtlich in zwei verschiedenen Städten sein werden, jedoch in einem Raum mit WLAN-Verbindung, und dass insgesamt 64 Potentiometer synchronisiert werden müssen, werden im Folgenden einige dieser Optionen diskutiert:

### MQTT (Message Queuing Telemetry Transport):

- **Sinnvoll:** MQTT ist ein leichtgewichtiges Protokoll, das eine zuverlässige und schnelle Datenübertragung ermöglicht. Es bietet eine einfache Möglichkeit, Daten zwischen verschiedenen Geräten zu übertragen, und unterstützt sowohl Publish-Subscribe- als auch Punkt-zu-Punkt-Kommunikation.
- **Nicht sinnvoll:** Möglicherweise ist eine zusätzliche Infrastruktur wie ein MQTT-Broker erforderlich, was zusätzlichen Aufwand für die Einrichtung und Wartung bedeutet. Die Verwendung von MQTT erfordert jedoch keine direkte Verbindung zwischen den ESP32-Boards und bietet eine flexible Möglichkeit, die Potentiometer-Positionen zu übertragen.

### Direkte drahtlose Kommunikation (z. B. Wi-Fi Direct):

- **Sinnvoll:** Direkte drahtlose Kommunikation ermöglicht eine einfache Punkt-zu-Punkt-Verbindung zwischen den beiden ESP32-Geräten, ohne dass zusätzliche Infrastruktur erforderlich ist. Dies könnte eine gute Option sein, wenn eine schnelle und zuverlässige Verbindung zwischen den Geräten erforderlich ist.
- **Nicht sinnvoll:** Die Reichweite von Wi-Fi Direct kann begrenzt sein, insbesondere in Umgebungen mit vielen Störquellen. Dies könnte problematisch sein, wenn sich die ESP32-Boards in verschiedenen Städten befinden.

### Bluetooth:

- **Sinnvoll:** Bluetooth bietet eine einfache Möglichkeit zur drahtlosen Datenübertragung zwischen zwei Geräten und ist weit verbreitet. Es ist eine gute Option für kurze bis mittlere Entfernungen und bietet eine einfache Einrichtung.
- **Nicht sinnvoll:** Möglicherweise ist eine zusätzliche Konfiguration erforderlich, und die Übertragungsgeschwindigkeit könnte langsamer sein als bei anderen Optionen. Zudem könnte die Reichweite von Bluetooth begrenzt sein, was bei der Übertragung zwischen verschiedenen Städten problematisch sein könnte.

### LoRaWAN (Long Range Wide Area Network):

- **Sinnvoll:** LoRaWAN bietet eine große Reichweite und eignet sich daher gut für die drahtlose Kommunikation über große Entfernungen. Es ist eine gute Option, wenn die ESP32-Geräte weit voneinander entfernt sind und eine stabile Verbindung benötigen.
- **Nicht sinnvoll:** Die Einrichtung eines LoRaWAN-Netzwerks erfordert möglicherweise zusätzliche Hardware und Infrastruktur, was zusätzlichen Aufwand bedeuten kann. Zudem könnte die Übertragungsgeschwindigkeit langsamer sein als bei anderen Optionen, was für Echtzeit-Anwendungen wie die Synchronisation von Potentiometern problematisch sein könnte.

### Fazit

Die Auswahl der geeigneten Methode zur Übertragung der Potentiometer-Positionen hängt von verschiedenen Faktoren ab, einschließlich der Anforderungen an Reichweite, Zuverlässigkeit, Komplexität und verfügbarer Infrastruktur. Angesichts der gegebenen Grundinformationen könnte eine WLAN-Verbindung eine sinnvolle Wahl sein, um die Implementierung eines zusätzlichen Brokers zu vermeiden, auch wenn MQTT eine gute Option wäre. Die anderen Optionen fallen aufgrund ihrer negativen Punkte aus, insbesondere in Bezug auf Reichweite und Konfigurationsaufwand.



## 5. Installation

### Verwendete Technologien

- **Arduino:** Verwendung von Arduino-Plattform und ESP32-Mikrocontroller.
- **WiFi-Bibliothek:** Verbindung zum WLAN-Netzwerk für die MQTT-Kommunikation.
- **PubSubClient:** Verwendung der PubSubClient-Bibliothek für die MQTT-Kommunikation.
- **MQTT-Protokoll:** Verbindung zu einem MQTT-Broker für die Nachrichtenübertragung.

### Funktionalitäten

- Steuerung der Motorfader durch MQTT-Nachrichten.
- Kommunikation mit einem MQTT-Broker zur Übertragung von Nachrichten.
- Überprüfung und Aktualisierung der Motorfader-Position basierend auf empfangenen Nachrichten.
- Unterstützung für manuelle und automatische Steuerung der Motorfader-Position.

### Installation und Konfiguration

1. Stellen Sie die Verbindungsdaten für Ihr WLAN-Netzwerk und Ihren MQTT-Broker in den Code ein.
2. Laden Sie den Code auf Ihren ESP32-Mikrocontroller hoch.
3. Führen Sie den Code auf dem ESP32-Mikrocontroller aus und überwachen Sie die Ausgabe, um den Verbindungsstatus und die Motorfader-Position zu überprüfen.

## 6. Ausblick / Erweiterungsmöglichkeiten

- Entwicklung und Evaluierung eines erweiterten Synchronisierten Motorfaders
- Sicherheitsanalyse des Kommunikationsprotokolls (z. B. MQTT) zur Identifizierung potenzieller Schwachstellen und Entwicklung von Sicherheitsmaßnahmen




[Aktivitätsmodell]:https://github.com/TheManitu/Interactive-Pinboard/blob/main/Artefakte/Architektur-Aktivitätsdiagramm.png
[Architekturmodell]:https://github.com/TheManitu/Interactive-Pinboard/blob/main/Artefakte/Architektur-Architekturdiagramm.png
[Skizze]:https://github.com/TheManitu/Synchronisierte-Motorfader-Kommunikation/blob/main/Artefakte/Skizze.png
[Expose]:https://github.com/TheManitu/Synchronisierte-Motorfader-Kommunikation/blob/main/Artefakte/hesse-manuel-pp-ss-2024.pdf
[Mindmap]:https://github.com/TheManitu/Synchronisierte-Motorfader-Kommunikation/blob/main/Artefakte/hesse-manuel-themenfeldmindmap.png
[Logik]:https://github.com/TheManitu/Synchronisierte-Motorfader-Kommunikation/blob/main/Artefakte/Logik.png


