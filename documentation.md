## Serial Studio + Arduino R4
### Côté Ordinateur
- Copier l'addresse ipv4 de l'ordinateur avec la commande ```ipconfig``` voir la section "Carte réseau sans fil Wi-Fi" 
- Activer le partage de connection sur l'ordinateur
  
### Côté Arduino 
- Mettre le code suivant au début du programme :
```
#include <WiFiS3.h>
#include <WiFiUdp.h>

const char* ssid = "RAPHAEL_6598";
const char* pass = "18c235=X";
const char* pcIp = "10.170.9.229" ;//"192.168.18.16"; 
const int port = 8888;

WiFiUDP Udp;
  ```
- Dans 
