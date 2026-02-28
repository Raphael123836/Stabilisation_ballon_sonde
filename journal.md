# Journal relatant les différents essais pour stabiliser un ballon-sonde sur l'axe Z

## Test 1 : Rigidité gyroscopique (Échec)

La première tentative consistait à faire tourner à haute vitesse (18 000 RPM) une roue d'inertie avec un moteur brushless.  
On tentait ainsi d'exploiter l'effet de rigidité gyroscopique.  

Pour tester le système, nous avons placé la plateforme à stabiliser avec des cordes afin qu'elle soit libre de mouvement. Nous avons ensuite utilisé un ventilateur que nous démarrions en même temps qu'un chronomètre pour mesurer le temps que cela prenait, pour chaque essai, à la plateforme pour effectuer un tour.  

Sans le système activé, la plateforme prenait 12 s, tandis qu'avec la roue à vitesse maximale, on était à 17,5 s, ce qui n'est pas suffisant.

### Voici le code qui permettait d'envoyer différentes vitesses à distance :

```cpp
#include <WiFiS3.h>
#include <WiFiUdp.h>
#include <Servo.h>

const char* ssid = "RAPHAEL_6598";
const char* pass = "18c235=X";
const int port_ecoute = 7777;

Servo ESC; 
WiFiUDP Udp;

char packetBuffer[255];
int speed = 0;

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

  ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds)
  delay(1000); 
  ESC.write(0); // Envoie le signal minimum
  delay(5000); 
}

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

  if (speed >= 0 and speed <= 180){
    //Serial.println(speed);  
      ESC.write(speed);    // Send the signal to the ESC value between 0 and 180
  }

}
