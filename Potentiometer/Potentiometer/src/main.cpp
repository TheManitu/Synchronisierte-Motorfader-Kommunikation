#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "DEIN_WIFI_SSID";
const char* password = "DEIN_WIFI_PASSWORT";
const char* mqtt_server = "IP_DES_MQTT_BROKERS";

WiFiClient espClient;
PubSubClient client(espClient);

const int potPin = 34; // Pin, an dem der Potentiometer angeschlossen ist
int lastPotValue = 0;   // Letzter gemessener Wert des Potentiometers

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
    } else {
      Serial.print("Fehlgeschlagen, rc=");
      Serial.print(client.state());
      Serial.println(" erneuter Versuch in 5 Sekunden");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Lese den Wert des Potentiometers
  int potValue = analogRead(potPin);

  // Wenn sich der Potentiometerwert geändert hat, sende ihn über MQTT
  if (potValue != lastPotValue) {
    Serial.print("Potentiometer-Wert: ");
    Serial.println(potValue);
    client.publish("potentiometer/value", String(potValue).c_str());
    lastPotValue = potValue;
  }

  delay(100); // Verzögerung für Stabilität und um die serielle Ausgabe zu verfolgen
}
