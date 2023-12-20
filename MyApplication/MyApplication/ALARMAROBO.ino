#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define trig 13 // Emisor de pulso o señal
#define echo 12 // Receptor "del eco" del pulso o señal
#define buzzer 15 // Zumbador

#define WIFI_SSID "vivo Y21s" // Nombre red wifi
#define WIFI_PASSWORD "1234567890" // Contraseña red wifi

unsigned long channelID = 2379033;
const char* WriteAPIKey = "MAWOVDXG0QX5X9XG";
const char* ReadAPIKey = "UKEDCPDLTPMWJCZ0"; // Clave de lectura

WiFiClient Client;

int alarmState = 0; // 0: Apagado, 1: Encendido, 2: Confirmación
unsigned long previousMillis = 0;
const long interval = 5000; // Intervalo de 2.5 segundos

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);

  // Conectar a la red WiFi
  Serial.println();
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(Client);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    HTTPClient https; // Declare an object of class HTTPClient
    client.setInsecure(); // Ignore SSL certificate errors

    long duration, distance;

    digitalWrite(trig, LOW);
    delay(2);
    digitalWrite(trig, HIGH);
    delay(5);
    digitalWrite(trig, LOW);

    duration = pulseIn(echo, HIGH);
    distance = (duration / 2) * 0.0343;

    Serial.println(distance);

    // Enviar datos a ThingSpeak
    ThingSpeak.setField(1, duration);
    ThingSpeak.setField(2, distance);

    // Leer datos de ThingSpeak
    String readURL = "https://api.thingspeak.com/channels/2379033/fields/3/last.json?api_key=UKEDCPDLTPMWJCZ0";
    https.begin(client, readURL);
    int httpCode = https.GET();

    if (httpCode > 0) {
      String payload = https.getString();
      Serial.println("Respuesta del servidor ThingSpeak: " + payload);

      // Parsear el JSON
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.println("Error al analizar JSON");
        return;
      }

      // Obtener el valor del campo 3 (o el que corresponda)
      long fieldValue = doc["field3"];
      Serial.println("Valor del campo 3: " + String(fieldValue));

      // Aplicar la condición después de obtener el valor del campo 3
      if (fieldValue == 1) {
        alarmState = 1;
        tone(buzzer, 1000);
      } else {
        alarmState = 0;
        noTone(buzzer);
      }

      ThingSpeak.setField(3, alarmState);
      ThingSpeak.writeFields(channelID, WriteAPIKey);
      Serial.println("Datos enviados a ThingSpeak!");

      // Resetear el tiempo
      previousMillis = millis();
    } else {
      Serial.println("Error en la solicitud HTTP");
    }

    // Esperar hasta que haya pasado el intervalo
    if (millis() - previousMillis >= interval) {
      delay(5000); // Asegurar un intervalo constante
    }
  }
}








 
