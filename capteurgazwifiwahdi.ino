#include <ESP8266WiFi.h>
 
const char* ssid = "AndroidAPD85D";
const char* password = "amine123";

int buzzer = 10;
int smokeA0 = 4;
int sensorThres = 400;

WiFiServer server(80);

void setup() {

  
  Serial.begin(115200);
  delay(100);
  
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);

  // Connexion wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
  
  // connection  en cours ...
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");

    // Wifi connecter
  Serial.println("WiFi connecter");
 
  // Démmarrage du serveur.
  server.begin();
  Serial.println("Serveur demarrer !");
  }

  // Affichage de l'adresse IP
  Serial.print("Utiliser cette adresse URL pour la connexion :");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 

}

void loop() 
{
WiFiClient client;

  
  // Vérification si le client est connecter.
  client = server.available();
  if (!client)
  {
    return;
  }
 
  // Attendre si le client envoie des données ...
  Serial.println("nouveau client");
  while(!client.available()){
    delay(1);
  }

   //String request = client.readStringUntil('\r');
  //Serial.println(request);
  //client.flush();
  
  int analogSensor = analogRead(smokeA0);

  Serial.print("Pin A5: ");
  Serial.println(analogSensor);
  if (analogSensor>200) {
   Serial.println("alerte detection de fuite de gaz");
  }else {
    Serial.println("pas de fuite de gaz");
  }
//  Checks if it has reached the threshold value
 if (analogSensor > sensorThres)
  {
   
    tone(buzzer, 1000, 200);
  }
  else
  {
   
   noTone(buzzer);
  }
  delay(1000);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("Etat de gaz : ");
  if (analogSensor>200) {
    client.print("<p> alerte detection de fuite de gaz </p> ");
  } else {
    client.print("<p> pas de fuite de gaz </p>");
  }

  delay(1);
  Serial.println("Client deconnecter");
  Serial.println("");
 
}
