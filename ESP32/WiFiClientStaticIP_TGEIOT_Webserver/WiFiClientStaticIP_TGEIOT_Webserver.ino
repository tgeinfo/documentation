/*
  Programme : Configuration d'une adresse IP statique avec modification de l'adresse MAC et serveur web sur ESP32
  Auteur : Maxime Gillespie 
  Date : 2024-06-16 (Mis à jour le 2024-10-23)

  Description :
  Ce programme permet à un ESP32 de se connecter à un réseau Wi-Fi en utilisant une adresse IP statique
  et de modifier son adresse MAC en mode station pour répondre aux besoins spécifiques du réseau. 
  De plus, il crée un serveur web simple sur le port 80 qui affiche un message de bienvenue lorsque 
  l'utilisateur se connecte à l'ESP32 via un navigateur web.

  Fonctionnalités du Programme :
  1. **Modification de l'Adresse MAC** :
     - La fonction `esp_wifi_set_mac()` est utilisée pour définir une nouvelle adresse MAC pour l'ESP32 avant d'établir la connexion Wi-Fi.
     - Cela permet de s'assurer que chaque appareil a une adresse MAC unique, évitant les conflits sur le réseau.
  2. **Configuration de l'Adresse IP Statique** :
     - La fonction `WiFi.config()` permet de configurer une adresse IP statique, une passerelle et des serveurs DNS.
     - Cette configuration est souvent nécessaire lorsque les adresses IP dynamiques (DHCP) sont restreintes ou pour des besoins de maintenance et d'accès prédictible aux appareils.
  3. **Connexion Wi-Fi** :
     - La fonction `WiFi.begin()` est utilisée pour se connecter à un réseau Wi-Fi spécifique avec un SSID et un mot de passe.
     - Une boucle `while` attend que l'ESP32 soit connecté avant de poursuivre l'exécution, garantissant ainsi que la connexion est établie avec succès avant de continuer.
  4. **Serveur Web sur le Port 80** :
     - Un serveur web est initialisé sur le port 80 et répond aux requêtes HTTP. Lorsqu'un utilisateur accède à l'ESP32 via son adresse IP, le serveur renvoie une page avec le message "Bonjour depuis le ESP32!".

  Attention :
  - Assurez-vous que la nouvelle adresse MAC est unique sur le réseau pour éviter les conflits.
  - La modification de l'adresse MAC doit être effectuée en respectant les politiques et réglementations en vigueur.
  - Ce code est destiné à des fins éducatives dans le cadre d'un cours sur la programmation des microcontrôleurs.

  Configuration réseau :
  - Adresse IP statique : 10.63.X.X
  - Passerelle : 10.63.0.1
  - Masque de sous-réseau : 255.255.0.0
  - DNS primaire : 10.2.0.211
  - DNS secondaire : 10.2.0.210

  Bibliothèques requises :
  - WiFi.h : pour les fonctionnalités Wi-Fi de base.
  - esp_wifi.h : pour accéder aux fonctions avancées Wi-Fi, notamment pour changer l'adresse MAC.
  - WebServer.h : pour gérer un serveur web simple sur l'ESP32.

  Licence :
  - Ce code est fourni à titre éducatif et peut être utilisé et modifié librement dans le cadre du cours.
*/


#include <WiFi.h>             // Bibliothèque pour les fonctionnalités Wi-Fi de base de l'ESP32
#include <WebServer.h>        // Bibliothèque pour gérer un serveur web simple

// Identifiants du réseau Wi-Fi à rejoindre
const char *ssid = "CPI-IOT";      // Nom du réseau Wi-Fi (SSID)
const char *password = "xxxxxx";  // Mot de passe du réseau Wi-Fi


// Déclaration du serveur web sur le port 80
WebServer server(80);

// Fonction de gestion des requêtes sur la page racine "/"
void handleRoot() {
  server.send(200, "text/html", "Bonjour depuis le ESP32!");
}

void setup() {
  Serial.begin(115200);  // Initialiser la communication série pour les messages de débogage

  // Configurer l'ESP32 en mode station (client Wi-Fi)
  WiFi.mode(WIFI_STA);

  // Débuter la connexion au réseau Wi-Fi
  Serial.print("Connexion au réseau ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Attendre que la connexion soit établie
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  // Indiquer que la connexion est en cours
  }

  // Une fois la connexion établie, afficher les informations du réseau
  Serial.println("\nConnexion Wi-Fi établie !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
  Serial.print("Adresse MAC de l'ESP : ");
  Serial.println(WiFi.macAddress());
  Serial.print("Masque de sous-réseau : ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Passerelle : ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS : ");
  Serial.println(WiFi.dnsIP());

  // Initialiser le serveur web
  server.on("/", handleRoot); // Associer la fonction handleRoot à la racine "/"
  server.begin();             // Démarrer le serveur web
  Serial.println("Serveur web démarré !");
}

void loop() {
  // Ecouter les requêtes HTTP entrantes
  server.handleClient();
}
