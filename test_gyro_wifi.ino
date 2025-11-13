#include <WiFiS3.h>   // Bibliothèque WiFi pour UNO R4 WiFi

// Nom du réseau créé par l'Arduino
const char* ssid = "R4_AP_Test";  

WiFiServer server(80);  // Serveur web sur port 80

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Démarrage...");

  // --- Mode Access Point ---
  Serial.print("Création de l'AP : ");
  Serial.println(ssid);

  int result = WiFi.beginAP(ssid);  // AP sans mot de passe

  if (result != WL_AP_LISTENING) {
    Serial.println("Erreur : impossible de créer l'AP !");
    while (true);
  }

  IPAddress ip = WiFi.localIP();
  Serial.print("AP actif ! Connectez-vous à : ");
  Serial.println(ssid);
  Serial.print("Adresse web à ouvrir : http://");
  Serial.println(ip);

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connecté !");

    // Attendre une requête
    while (client.connected() && !client.available()) {
      delay(1);
    }

    // Lire la requête
    String req = client.readStringUntil('\r');
    Serial.print("Requête reçue : ");
    Serial.println(req);

    // --- Réponse HTML ---
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE html><html>");
    client.println("<h1>Arduino UNO R4 WiFi</h1>");
    client.println("<p>Voici les prints envoyés depuis l'Arduino !</p>");
    client.println("<p>voici </p>");
    client.println("</html>");

    delay(1);
    client.stop();
    Serial.println("Client déconnecté.\n");
  }
}
