## Serial Studio + Arduino R4
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
iFi.begin(ssid, pass);
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


