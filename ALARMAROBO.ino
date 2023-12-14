#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define trig 13 // Emisor de pulso o señal
#define echo 12 // Receptor "del eco" del pulso o señal
#define buzzer 15 // Zumbador

#define WIFI_SSID "vivo Y21s" //Nombre red wifi
#define WIFI_PASSWORD "1234567890" //Pass red wifi

unsigned long channelID = 2379033;
const char* WriteAPIKey = "MAWOVDXG0QX5X9XG";

WiFiClient Client;

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
  //Desde aca incorpora tus sentencias
}

void loop() {
   // Bucle
     if (WiFi.status() == WL_CONNECTED) {
     WiFiClientSecure client;
    HTTPClient https;  //Declare an object of class HTTPClient
    client.setInsecure(); // Ignore SSL certificate errors

  
  long duration, distance; //Establecemos duration y distance como variables numéricas extensas

  digitalWrite(trig, LOW); //Para tener un pulso limpio empezamos con 2 microsegundos en apagado

  delay(2);

  digitalWrite(trig, HIGH); //Mandamos un pulso de 5 microsegundos

  delay(5);

  digitalWrite(trig, LOW); //Apagamos

  duration = pulseIn(echo, HIGH); //Medimos el tiempo que la señal tarda en volver al sensor en microsegundos

  distance = (duration/2)*0.0343; //La distancia es el tiempo por la velocidad del sonido (343 m/s = 0.0343 cm/microseg)

  Serial.println(distance);

  // Añade las siguientes líneas para enviar duración y distancia a ThingSpeak
  ThingSpeak.setField(1, duration); // Ajusta el número de campo según tu preferencia
  ThingSpeak.setField(2, distance); // Ajusta el número de campo según tu preferencia

  if (distance < 10) //Si la distancia es menor de un metro y medio
  
{ tone(buzzer, 1000); //Suena el zumbador con una frecuencia de 1000Hz

  delay(2500); //durante 2.5 segundos
}
else //De lo contrario
{
  noTone(buzzer);//no suena
  }

  ThingSpeak.writeFields(channelID, WriteAPIKey);
  Serial.println("Datos enviados a ThingSpeak!");

  delay(14000);
}
}






 
