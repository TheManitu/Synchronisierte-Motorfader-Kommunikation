#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WifiClientSecure.h>
 
  //WIFI Connection
    const char* ssid = "YourID";
    const char* password = "YourPW";

//MQTT Connection
    const char* broker = "Your-BrokerID";
    const char* mqttUser = "";
    const char *clientIdPrefix = "02_"; // Prefix für die Client-ID, um den ESP zu identifizieren

   const char* mqttPassword = "";
    const int mqtt_port= 1883;
    const char* mqttpubTopic = "position/poti1";
    const char* mqttsubTopic = "position/poti2";

 // Secure WiFi Connection
WiFiClient espClient;

// MQTT Client using WiFi Connection
PubSubClient client(espClient);

int value = 0;
String receivedMessage;
int receivedValue = 0;
int prevValue=0;


// Pin Definitions
#define potPin 34
#define enA 25       
#define motorUp 14   
#define motorDown 13 

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

// Globale Variable für den Durchschnitt des Potentiometerwerts
int potiAverage = 0;

// Anzahl der Messungen für den Moving Average
const int numReadings = 10;
int potiReadings[numReadings];  // Array zum Speichern der letzten Messwerte
int potiIndex = 0;               // Index des aktuellen Messwerts

// Funktion zum Lesen des Potentiometerwerts und Berechnen des Durchschnitts
void readAndAveragePotentiometer() {
    // Aktuellen Potentiometerwert lesen
    int potival = analogRead(potPin);

    // Neuen Messwert zum Array hinzufügen und den ältesten entfernen
    potiReadings[potiIndex] = potival;
    potiIndex = (potiIndex + 1) % numReadings;

    // Moving Average berechnen
    potiAverage = 0;
    for (int i = 0; i < numReadings; i++) {
        potiAverage += potiReadings[i];
    }
    potiAverage /= numReadings;
}


void publishPotiValue(int potiAverage)
{
  // Potentiometerwert an das MQTT-Thema veröffentlichen
  client.publish(mqttpubTopic, String(potiAverage).c_str(), true);

  Serial.println("Potentiometerwert veröffentlicht: " + String(potiAverage));
}


// Annahme: Globale Variable, um den letzten veröffentlichten Wert zu speichern
int lastPublishedValue = 0;

// Funktion zum Senden der aktuellen Position mit Überprüfung der Änderung
void sendCurrentPosition() {
    readAndAveragePotentiometer();

    // Überprüfen, ob der neue Wert sich um mehr als 100 von dem letzten Wert unterscheidet
    if (abs(potiAverage - lastPublishedValue) > 200) {
        publishPotiValue(potiAverage); // Veröffentlicht den neuen Wert
        lastPublishedValue = potiAverage; // Aktualisiert den letzten veröffentlichten Wert
    }
}

void movetoPosition(int targetPosition) {
    static int prevPosition = 0; // Variable, um die vorherige Position zu speichern
    static int prevTargetPosition = -1; // Variable, um die vorherige Zielposition zu speichern, initialisiert mit einem Wert, der sicher nicht mit einer gültigen Zielposition übereinstimmt
    readAndAveragePotentiometer();
    digitalWrite(motorDown, LOW); // Stelle sicher, dass beide Richtungen des Motors zuerst ausgeschaltet sind
    digitalWrite(motorUp, LOW);
    
    if (targetPosition != prevTargetPosition) {
        // Wenn sich die Zielposition geändert hat, setzen wir die vorherige Position zurück
        prevPosition = 0;
        prevTargetPosition = targetPosition;
    }

if ((potiAverage > targetPosition) && (potiAverage > targetPosition + 3000)) {
    // Bedingungen für potiAverage größer als 3000
    manuell = false;
    digitalWrite(motorDown, HIGH); // Motor in Richtung "Down" bewegen
    delay(10); // Motor für 1 Sekunde einschalten
    //digitalWrite(motorDown, LOW);
} 
else if ((potiAverage < targetPosition) && (potiAverage < targetPosition - 3000)) {
    // Bedingungen für potiAverage kleiner als 3000
    manuell = false;
    digitalWrite(motorUp, HIGH); // Motor in Richtung "Up" bewegen
    delay(10); // Motor für 1 Sekunde einschalten
    //digitalWrite(motorUp, LOW);
} 
else if ((potiAverage > targetPosition) && (potiAverage > targetPosition + 2000)) {
    // Bedingungen für potiAverage größer als 2000
    manuell = false;
    digitalWrite(motorDown, HIGH); // Motor in Richtung "Down" bewegen
    delay(10); 
    //(motorDown, LOW);
} 
else if ((potiAverage < targetPosition) && (potiAverage < targetPosition - 2000)) {
    // Bedingungen für potiAverage kleiner als 2000
    manuell = false;
    digitalWrite(motorUp, HIGH); // Motor in Richtung "Up" bewegen
    delay(8); 
    //digitalWrite(motorUp, LOW);
} 

else if ((potiAverage > targetPosition) && (potiAverage > targetPosition + 1000)) {
    // Bedingungen für potiAverage größer als 2000
    manuell = false;
    digitalWrite(motorDown, HIGH); // Motor in Richtung "Down" bewegen
    delay(4); 
    digitalWrite(motorDown, LOW);
    delay(5);
    //readAndAveragePotentiometer();
} 
else if ((potiAverage < targetPosition) && (potiAverage < targetPosition - 1000)) {
    // Bedingungen für potiAverage kleiner als 2000
    manuell = false;
    digitalWrite(motorUp, HIGH); // Motor in Richtung "Up" bewegen
    delay(4); 
    digitalWrite(motorUp, LOW);
    delay(5);
    //readAndAveragePotentiometer();
} 
    if ((potiAverage > targetPosition) && (potiAverage > targetPosition + 200)) {
        manuell = false;
        digitalWrite(motorDown, HIGH); // Wenn die Zielposition nicht erreicht ist, fahre den Motor
        delay(2);
        digitalWrite(motorDown, LOW);
        delay(5);
        //readAndAveragePotentiometer();
    } 
    else if ((potiAverage < targetPosition) && (potiAverage < targetPosition - 200)) {
      manuell = false;
        digitalWrite(motorUp, HIGH); // Wenn die Zielposition nicht erreicht ist, fahre den Motor an
        delay(2);
        digitalWrite(motorUp, LOW);
        delay(5);
        //readAndAveragePotentiometer();
    }
    
    
    if ((potiAverage > targetPosition - 200) && (potiAverage < targetPosition + 200)) {
        // Wenn die aktuelle Position innerhalb von +/- 50 der Zielposition liegt, stoppe den Motor
        digitalWrite(motorDown, LOW);
        digitalWrite(motorUp, LOW);
        //readAndAveragePotentiometer();
        //delay(50);
        sendCurrentPosition();
        manuell = true;
    }
}




void callback(char *topic, byte *payload, unsigned int length)
{
  //delay(100);
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


void positionChanged() {
    int prevPosition = analogRead(potPin); 
    // Verzögern für 200 Millisekunden
    delay(200);

    // Aktuelle Position erneut lesen
    int currentPosition = analogRead(potPin);

    // Überprüfen, ob sich die Position um mindestens 100 Einheiten geändert hat
    if (abs(currentPosition - prevPosition) >= 200) {
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
  readAndAveragePotentiometer();
  
  
if (manuell) {
    int prevValue = received_message.payload.toInt();
    readAndAveragePotentiometer();

    // Überprüfen, ob sich der potiAverage um +/- 200 vom letzten Publish unterscheidet
    if (abs(potiAverage - prevValue) >= 200) {
        // Überprüfen, ob sich die Position geändert hat
        Serial.println("aktuelle Position:" + String(potiAverage)); 
          positionChanged();
          if(!inBewegung){
            // Wenn sich die Position geändert hat, veröffentliche die neue Position
            Serial.println("In manuell-schleife: " + String(prevValue) + " + " + String(potiAverage));
            readAndAveragePotentiometer();
            //delay(50);
            sendCurrentPosition();
            inBewegung= true;
            // Optional: Warten, bevor fortgefahren wird
            //delay(5000);
          }
    }
}

if(!manuell){
  // Prüfen, ob eine neue Nachricht empfangen wurde
  if(abs(potiAverage - received_message.payload.toInt()) > 200) {
    // Int-Variable des anderen Sliders
    int sliderPosition = received_message.payload.toInt();
    // Funktion um Position anzufahren
    movetoPosition(sliderPosition);
    //Serial.println("In Schleife nach movetoPosition");
  }
  if(abs(potiAverage - received_message.payload.toInt()) < 200){
    manuell= true;
  }
  // CASE: wenn nach movetoPosition erkannt wird, dass Ziel bereits erreicht wurde 
  if (abs(potiAverage - received_message.payload.toInt()) < 200) {
        // Wenn die aktuelle Position innerhalb von +/- 50 der Zielposition liegt, stoppe den Motor
        digitalWrite(motorDown, LOW);
        digitalWrite(motorUp, LOW);
        Serial.println("Reached target position");
        //prev steht für Position vor dem manuellen bewegen
        readAndAveragePotentiometer();
        sendCurrentPosition();
        manuell = true;
        
    }
  }
}