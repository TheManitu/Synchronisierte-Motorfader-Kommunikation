#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WifiClientSecure.h>
 
  //WIFI Connection
    //const char* ssid = "Manuel";
    //const char* password = "mobilpw1";
    const char* ssid = "moxd-lab-iot";
    const char* password = "xyv176935";

  //MQTT Connection
    const char* broker = "hivemq.dock.moxd.io";
    const char* mqttUser = "";
    const char *clientIdPrefix = "02_"; // Prefix für die Client-ID, um den ESP zu identifizieren

    //const char* broker = "mqtt.flespi.io";
    //const char* mqttUser = "ByYAWGNkH0JlvBBVKMMwtqtSYHQaGQv7ckwWO96kYDvj5X2FxD0QRjwGObEkRZgR";
    const char* mqttPassword = "";
    const int mqtt_port= 1883;
    const char* mqttpubTopic = "position/poti1";
    const char* mqttsubTopic = "position/poti2";

 // Secure WiFi Connection
WiFiClient espClient;

// MQTT Client using WiFi Connection
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;
String receivedMessage;
int receivedValue = 0;
int prevValue=0;


// Pin Definitions
#define potPin 34
#define enA 25       // 5
#define motorUp 14   // 9
#define motorDown 13 // 8

// Parameters
int potival = 0;
int prevPotival = 0;
bool manuell= true;
bool inBewegung = true;

struct MqttMessage
{
  String topic;
  String payload;
};

MqttMessage received_message; // Globale Variable zum Speichern des empfangenen Topics und der Nachricht

void setupWifi()
{
  // Connect to Wi-Fi
  delay(10);
  Serial.println("Connecting to");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Verbindung zum MQTT-Broker...");
    if (client.connect((String(clientIdPrefix) + "ESP32").c_str(), mqttUser, mqttPassword))

    {
      Serial.println("verbunden");
      client.subscribe(mqttsubTopic);
    }
    else
    {
      Serial.print("Fehlgeschlagen, rc=");
      Serial.print(client.state());
      Serial.println("Warte 5 Sekunden, bevor ein erneuter Versuch unternommen wird...");
      delay(5000);
    }
  }
}

void publishPotiValue(int potiValue)
{
  // Potentiometerwert an das MQTT-Thema veröffentlichen
  client.publish(mqttpubTopic, String(potiValue).c_str(), true);

  Serial.println("Potentiometerwert veröffentlicht: " + String(potiValue));
}

// Annahme: Globale Variable, um den letzten veröffentlichten Wert zu speichern
int lastPublishedValue = 0;

// Funktion zum Senden der aktuellen Position mit Überprüfung der Änderung
void sendCurrentPosition() {
    int potival = analogRead(potPin);

    // Überprüfen, ob der neue Wert sich um mehr als 100 von dem letzten Wert unterscheidet
    if (abs(potival - lastPublishedValue) > 100) {
        publishPotiValue(potival); // Veröffentlicht den neuen Wert
        lastPublishedValue = potival; // Aktualisiert den letzten veröffentlichten Wert
    }
}

void movetoPosition(int targetPosition) {
    static int prevPosition = 0; // Variable, um die vorherige Position zu speichern
    static int prevTargetPosition = -1; // Variable, um die vorherige Zielposition zu speichern, initialisiert mit einem Wert, der sicher nicht mit einer gültigen Zielposition übereinstimmt
    int potival = analogRead(potPin); // Aktuelle Position des Motors ablesen
    digitalWrite(motorDown, LOW); // Stelle sicher, dass beide Richtungen des Motors zuerst ausgeschaltet sind
    digitalWrite(motorUp, LOW);
    
    if (targetPosition != prevTargetPosition) {
        // Wenn sich die Zielposition geändert hat, setzen wir die vorherige Position zurück
        prevPosition = 0;
        prevTargetPosition = targetPosition;
    }

    if ((potival > targetPosition) && (potival > targetPosition + 100)) {
        manuell = false;
        digitalWrite(motorDown, HIGH); // Wenn die Zielposition nicht erreicht ist, fahre den Motor
        delay(1);
        digitalWrite(motorDown, LOW);
        //delay(1);
    } 
    else if ((potival < targetPosition) && (potival < targetPosition - 100)) {
      manuell = false;
        digitalWrite(motorUp, HIGH); // Wenn die Zielposition nicht erreicht ist, fahre den Motor an
        delay(1);
        digitalWrite(motorUp, LOW);
        //delay(1);
    }
    
    if ((potival > targetPosition - 100) && (potival < targetPosition + 100)) {
        // Wenn die aktuelle Position innerhalb von +/- 50 der Zielposition liegt, stoppe den Motor
        digitalWrite(motorDown, LOW);
        digitalWrite(motorUp, LOW);
        Serial.println("Reached target position");
        //prev steht für Position vor dem manuellen bewegen
        int potival = analogRead(potPin); // Speichern Sie den aktuellen Potentiometerwert, um ihn später zu überprüfen
        sendCurrentPosition();
        //Serial.print("automatisches publishen");
        manuell = true;
    }
}


void callback(char *topic, byte *payload, unsigned int length)
{
  delay(100);
  Serial.print("Empfangen [");
  Serial.print(topic);
  Serial.print("]: ");

  received_message.topic = topic;

  received_message.payload = ""; // Leeren Sie die Variable, bevor Sie neue Daten speichern

  for (int i = 0; i < length; i++)
  {
    received_message.payload += (char)payload[i];
  }
  Serial.println(received_message.payload);
  manuell = false;
}



void setup()
{

  Serial.begin(115200);
  setupWifi();

  client.setServer(broker, mqtt_port);
  client.setCallback(callback);

  // Set the motor pins as outputs
  pinMode(potPin, INPUT);
  pinMode(enA, OUTPUT);
  pinMode(motorUp, OUTPUT);
  pinMode(motorDown, OUTPUT);
}

void checkPotiLimits()
{
  int potiValue = analogRead(potPin); // Potentiometerwert lesen

  // Überprüfen, ob der Potentiometerwert außerhalb der Grenzen liegt
  if (potiValue < 100 || potiValue > 4000)
  {
    // Wenn außerhalb der Grenzen, MotorUp und MotorDown auf LOW setzen
    digitalWrite(motorDown, LOW);
    digitalWrite(motorUp, LOW);
  }
}

void positionChanged() {
    int prevPosition = analogRead(potPin); 
    // Verzögern für 500 Millisekunden
    delay(50);

    // Aktuelle Position erneut lesen
    int currentPosition = analogRead(potPin);

    // Überprüfen, ob sich die Position um mindestens 100 Einheiten geändert hat
    if (abs(currentPosition - prevPosition) >= 100) {
        inBewegung= true; // Position hat sich ausreichend geändert
    } else {
        inBewegung= false; // Position hat sich nicht mehr ausreichend geändert
    }
}



void loop()
{
  // Überprüfe, ob der Client verbunden ist
  if (!client.connected())
  {
    reconnect();
  }

  // Subcribe und Callback des topics
  client.loop();
  // Unsubscribe direkt dem Callback der neuen Nachricht damit neue Nachrichten wieder empfangen werden
  //(Es bestand ein Fehler das durch das dauerhafte subscriben keine Nachricht ankam)
  //client.unsubscribe(mqttsubTopic);

  // Potentiometerwert lesen, anzeigen und publishen
  int potival = analogRead(potPin);
  
  
if (manuell) {
    int prevValue = received_message.payload.toInt();
    int potival = analogRead(potPin);

    // Überprüfen, ob sich der potival um +/- 200 vom letzten Publish unterscheidet
    if (abs(potival - prevValue) >= 200) {
        // Überprüfen, ob sich die Position geändert hat
        Serial.println("aktuelle Position:" + String(potival)); 
          positionChanged();
          if(!inBewegung){
            // Wenn sich die Position geändert hat, veröffentliche die neue Position
            Serial.println("In manuell-schleife: " + String(prevValue) + " + " + String(potival));
            int potival = analogRead(potPin);
            sendCurrentPosition();
            inBewegung= true;
            // Optional: Warten, bevor fortgefahren wird
            //delay(5000);
          }
    }
}

if(!manuell){
  // Prüfen, ob eine neue Nachricht empfangen wurde
  if(abs(potival - received_message.payload.toInt()) > 100) {
    // Int-Variable des anderen Sliders
    int sliderPosition = received_message.payload.toInt();
    // Funktion um Position anzufahren
    movetoPosition(sliderPosition);
    //Serial.println("In Schleife nach movetoPosition");
  }
  if(abs(potival - received_message.payload.toInt()) < 100){
    manuell= true;
  }
  // CASE: wenn nach movetoPosition erkannt wird, dass Ziel bereits erreicht wurde 
  if (abs(potival - received_message.payload.toInt()) < 100) {
        // Wenn die aktuelle Position innerhalb von +/- 50 der Zielposition liegt, stoppe den Motor
        digitalWrite(motorDown, LOW);
        digitalWrite(motorUp, LOW);
        Serial.println("Reached target position");
        //prev steht für Position vor dem manuellen bewegen
        int potival = analogRead(potPin); // Speichern Sie den aktuellen Potentiometerwert, um ihn später zu überprüfen
        sendCurrentPosition();
        manuell = true;
    }
  }
}