#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "DEIN_WIFI_SSID";
const char* password = "DEIN_WIFI_PASSWORT";
const char* mqtt_server = "IP_DES_MQTT_BROKERS";

WiFiClient espClient;
PubSubClient client(espClient);

const int motorPin = 16;      // GPIO-Pin, an den der Motor angeschlossen ist
const int potentiometerPin = 34;  // Analoger Eingangspin, an den der Potentiometer-Abgriff angeschlossen ist

int lastPotValue = 0;   // Letzte empfangene Position des Potentiometers

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Verbindung zum WiFi-Netzwerk: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi-Verbindung hergestellt");
  Serial.println("IP-Adresse: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Verbindung zum MQTT-Broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("verbunden");
      client.subscribe("potentiometer/value");
    } else {
      Serial.print("Fehlgeschlagen, rc=");
      Serial.print(client.state());
      Serial.println(" erneuter Versuch in 5 Sekunden");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Nachricht empfangen [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Konvertiere den empfangenen Payload (String) in eine Zahl
  String payloadString = (char*)payload;
  int receivedPotValue = payloadString.toInt();

  // Bewege den Motorfader entsprechend der empfangenen Position des Potentiometers
  analogWrite(motorPin, receivedPotValue / 16); // Anpassung des Wertebereichs 0-4095 auf 0-255 für analogWrite
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
