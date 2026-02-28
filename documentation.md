## Serial Studio + Arduino R4

## Réception seulement : 
### Côté Ordinateur
- Copier l'addresse ipv4 de l'ordinateur avec la commande ```ipconfig``` voir la section "Carte réseau sans fil Wi-Fi" 
- Activer le partage de connection sur l'ordinateur
  
### Côté Arduino 
- Mettre le code suivant au début du programme coller l'addresse Ip copier plus tôt:
```Cpp
#include <WiFiS3.h>
#include <WiFiUdp.h>

const char* ssid = "RAPHAEL_6598";
const char* pass = "18c235=X";
const char* pcIp = "Coller ici" ;//  "10.170.9.229" ou "192.168.18.16"; 
const int port = 8888;

WiFiUDP Udp;
  ```
- Dans la section setup mettre le code suivant
```Cpp
wiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Udp.begin(port);
  Serial.println("\nPrêt ! Envoi UDP en cours...");
  ```
- Concaténer un string pour faire l'envoie d'un seul string en mode CSV de cette façon :
```Cpp
  String frame = "";
  frame += String(10 * sin(0.15 * adc4)) + ",";
  frame += String(10 * sin(0.4 * adc5)) + "\n"; // Le \n est crucial pour finir la ligne
  ```
- Envoie du paquet
```Cpp
  Udp.beginPacket(pcIp, port);
  Udp.print(frame);
  Udp.endPacket();
  ```

### Côté Serial Studio
Étape pour configurer la connection UDP
- Dans l'onglet configuration de l'appareil (à droite)
    - Sélectionner graphique rapide
    - Dans "configuration de l'appareil" sélectionner socket réseau
    - Type de socket : udp
    - Port local et distant : 8888
    - adresse distante : laisser vide



## Réception et transmission (pas encore fonctionel avec uniquement serial studio): 
### Côté Ordinateur
- Activer le partage de connection sur l'ordinateur
- Copier l'addresse ipv4 de l'ordinateur avec la commande ```ipconfig``` voir la section "Carte réseau sans fil Connexion au réseau local* 2" 

### Côté Arduino 
- Mettre le code suivant au début du programme coller l'addresse Ip copier plus tôt:
```Cpp
#include <WiFiS3.h>
#include <WiFiUdp.h>

const char* ssid = "RAPHAEL_6598";
const char* pass = "18c235=X";
const char* pcIp = "192.168.137.1" ;//"192.168.18.16"; 
const int port_ecoute = 7777;
const int port_envoie = 9000;

WiFiUDP Udp;
  ```
- Dans la section setup mettre le code suivant
```Cpp
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Udp.begin(port_ecoute);
  Serial.println("\nPrêt ! Envoi UDP en cours...");
  delay(1000);
  Serial.println(WiFi.localIP());
  ```
- Exemple de code : 
```Cpp
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include <Servo.h>

Servo ESC; 

const char* ssid = "RAPHAEL_6598";
const char* pass = "18c235=X";
const char* pcIp = "192.168.137.1" ;//"192.168.18.16"; 
const int port_ecoute = 7777;
const int port_envoie = 9000;

WiFiUDP Udp;

int adc0, adc1, adc2, adc3, adc4, adc5 = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Udp.begin(port_ecoute);
  Serial.println("\nPrêt ! Envoi UDP en cours...");
  delay(1000);
  Serial.println(WiFi.localIP());

  ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds)
  ESC.write(0); // Envoie le signal minimum
  delay(3000); 
}

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

int speed = 0;
void loop() {

  int packetSize = Udp.parsePacket();
  if (packetSize) {

    Serial.print("Received packet of size ");

    Serial.println(packetSize);
    int rep = Udp.read();
    char c = (char)rep;
    Serial.print("Caractère reçu : ");
    Serial.println(c);

    speed = (int)c;
  }

  if (speed >=0 or speed <= 180){   
      ESC.write(speed);    // Send the signal to the ESC value between 0 and 180
     }

  currentMillis = millis();
  if (currentMillis - previousMillis >= 20) {
    previousMillis = currentMillis;
    // On incrémente tes compteurs pour les sinus
    adc0++; adc1+=2; adc2+=3; adc3+=4; adc4+=5; adc5+=6;


    // On construit la chaîne EXACTEMENT comme ton Serial.print
    String frame = "";
    frame += String(10 * sin(0.05 * adc0)) + ",";
    frame += String(10 * sin(0.1 * adc1)) + ",";
    frame += String(10 * sin(0.15 * adc2)) + ",";
    frame += String(10 * sin(0.2 * adc3)) + ",";
    frame += String(10 * sin(0.15 * adc4)) + ",";
    frame += String(10 * sin(0.4 * adc5)) + "\n"; // Le \n est crucial pour finir la ligne


    // On envoie le paquet
    
    Udp.beginPacket(pcIp, port_envoie);
    Udp.print(frame);
    Udp.endPacket();
  } 

}
  ```


### Côté Serial Studio 
Étape pour configurer la connection UDP (fonctionne que pour la réception présentement)
- Dans l'onglet configuration de l'appareil (à droite)
    - Sélectionner graphique rapide
    - Dans "configuration de l'appareil" sélectionner socket réseau
    - Type de socket : udp
    - Port local : 9000
    - Port distant : 7777
    - adresse distante : 192.168.137.3 // adresse ipv4 de l'arduino

      
# Envoyer des packet avec packets sender (solution B pour envoyer des packets)
  Librairies essentiel : 
  ```Cpp
  #include <WiFiS3.h>
  #include <WiFiUdp.h>
  ```
  Connection au hotspot de l'ordinateur : 
  ```Cpp 
  const char* ssid = "RAPHAEL_6598";
  const char* pass = "18c235=X";
  const int port_ecoute = 7777;
  ```
  Initialisation d'un tableau de char pour récupérer la donnée ```char packetBuffer[255]; ```
  
  Setup : 
  ```Cpp
  void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Udp.begin(port_ecoute);
    Serial.println("\nPrêt !");
    delay(1000);
    Serial.println(WiFi.localIP());
  }
  ```

loop :
 ``` cpp
void loop() {

  int packetSize = Udp.parsePacket();
  
  if (packetSize) {

    int len = Udp.read(packetBuffer, 255);

    if (len > 0) {

      packetBuffer[len] = 0;

    }

    Serial.println("Speed:");
    Serial.print(packetBuffer);
    
    speed = atoi(packetBuffer);
    Serial.println(speed);
  }
 ```



