#include <Arduino.h>
#include <ESP32Servo.h>
#include <esp32-hal-touch.h>
#include <PubSubClient.h>
#include<WiFi.h>

static const int servoPin = 16;
int posDegrees = 0;
Servo myservo;
const int touchPin = 18;


// MQTT Broker Konfiguration
const char* ssid = "FRITZ!Box 6660 Cable QC";
const char* pass = "24984708340581411420";
const char* mqttServer = "192.168.178.25";
const int mqttPort = 1883;
const char* mqttUser = "manu";
const char* mqttPassword = "test";
const char* servoTopic = "servo/position";

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{

Serial.begin(115200);

    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Verbindung zum WLAN wird hergestellt...");
    }

    client.setServer(mqttServer, mqttPort);

    Serial.begin(115200);
    myservo.attach(servoPin);
    myservo.write(180);
    delay(2000);
    myservo.write(160);
    delay(2000);
    myservo.write(140);
    delay(2000);
    myservo.write(120);
    delay(2000);
    myservo.write(100);
    delay(2000);
    myservo.write(80);
    delay(2000);
    myservo.write(70);
    delay(2000);
    myservo.write(80);
    delay(2000);
    myservo.write(100);
    delay(2000);
    myservo.write(120);
    delay(2000);
    myservo.write(140);
    myservo.write(160);
    delay(2000);
    myservo.write(180);


    
}

void reconnect() {
    while (!client.connected()) {
        Serial.println("Verbindung zum MQTT-Broker wird hergestellt...");
        if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
            Serial.println("Verbunden mit dem MQTT-Broker");
            client.subscribe(servoTopic);
        } else {
            Serial.print("Fehler beim Verbinden mit dem MQTT-Broker. Wiederholung in 5 Sekunden...");
            delay(5000);
        }
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Nachricht auf Topic [");
    Serial.print(topic);
    Serial.print("]: ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // Hier  Servocode einfügen
}

void sendServoPosition(int position) {
    char message[10];
    sprintf(message, "%d", position);
    client.publish(servoTopic, message);
}

     
/*
Touch Sensor Test 
int touchValue = touchRead(touchPin);
if (touchValue < 50) {
        Serial.println("Touch erkannt!");
        // Hier können Sie Code für die Reaktion auf den Touch einfügen
    }

    delay(100);  // Kleine Verzögerung, um Störungen zu vermeiden
*/

/* 
 void pin_oben()
{
    Serial.printf("\npin_oben wurde aufgerufen, aktuelle Position Servo: %i\n" ,posDegrees);
    if (posDegrees >= 90){
        Serial.println("Pin ist oben!");
    } else {
         for (posDegrees = 0; posDegrees < 90; posDegrees += 1) {    
            myservo.write(posDegrees); 
        }
        Serial.println("\n... Pin wird hoch gefahren ...\n");
    }
    Serial.printf("\npin_oben wurde ausgeführt, aktuelle Position Servo: %i\n\n" ,posDegrees);
    // delay(10000);
}

void pin_unten()
{
    Serial.printf("\npin_unten wurde aufgerufen, aktuelle Position Servo: %i\n" ,posDegrees);
    if (posDegrees == 0){
        Serial.println("Pin ist unten!");
    } else {
        for (posDegrees = 90; posDegrees > 0; posDegrees -= 1) {
            myservo.write(posDegrees);
        }
    }
    Serial.println("\n... Pin wird herunter gefahren ...\n");
    Serial.printf("\npin_unten wurde ausgeführt, aktuelle Position Servo: %i\n\n" ,posDegrees);
    // delay(10000);
}
 */

