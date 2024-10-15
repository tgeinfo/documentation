/*
  Programme : Configuration d'une adresse IP statique avec modification de l'adresse MAC sur ESP32
  Auteur : Maxime Gillespie 
  Date : 2024-06-16

  Description :
  Ce programme permet à un ESP32 de se connecter à un réseau Wi-Fi en utilisant une adresse IP statique.
  Il modifie également l'adresse MAC de l'ESP32 en mode station pour répondre aux besoins spécifiques du réseau.
  Ceci est utile dans le cadre de laboratoires où la réutilisation d'adresses IP est limitée par le réseau du cégep.

  Fonctionnalités du Programme :
  1. **Modification de l'Adresse MAC** :
     - La fonction `esp_wifi_set_mac()` est utilisée pour définir une nouvelle adresse MAC pour l'ESP32 avant d'établir la connexion Wi-Fi.
     - Cela permet de s'assurer que chaque appareil a une adresse MAC unique, évitant les conflits sur le réseau, ce qui est essentiel dans un environnement comme un laboratoire.
  2. **Configuration de l'Adresse IP Statique** :
     - La fonction `WiFi.config()` permet de configurer une adresse IP statique, une passerelle, et des serveurs DNS.
     - Cette configuration est souvent nécessaire lorsque les adresses IP dynamiques (DHCP) sont restreintes ou pour des besoins de maintenance et d'accès prédictible aux appareils.
  3. **Connexion Wi-Fi** :
     - La fonction `WiFi.begin()` est utilisée pour se connecter à un réseau Wi-Fi spécifique avec un SSID et un mot de passe.
     - Une boucle `while` est présente pour attendre que l'ESP32 soit connecté avant de poursuivre l'exécution, garantissant ainsi que la connexion est établie avec succès avant de continuer.

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

  Licence :
  - Ce code est fourni à titre éducatif et peut être utilisé et modifié librement dans le cadre du cours.
*/

#include <WiFi.h>             // Bibliothèque pour les fonctionnalités Wi-Fi de base de l'ESP32
#include "esp_wifi.h"        // Bibliothèque pour accéder aux fonctions avancées, comme la modification de l'adresse MAC

// Identifiants du réseau Wi-Fi à rejoindre
const char *ssid = "TGE-IOT";      // Nom du réseau Wi-Fi (SSID)
const char *password = "XXXXXXX";  // Mot de passe du réseau Wi-Fi

// Configuration de l'adresse IP statique
IPAddress local_IP(10, 63, 90, 80);         // Adresse IP statique assignée à l'ESP32
IPAddress gateway(10, 63, 0, 1);            // Adresse de la passerelle du réseau
IPAddress subnet(255, 255, 0, 0);           // Masque de sous-réseau
IPAddress primaryDNS(10, 2, 0, 211);        // Adresse du serveur DNS principal (optionnel)
IPAddress secondaryDNS(10, 2, 0, 210);      // Adresse du serveur DNS secondaire (optionnel)

// Nouvelle adresse MAC à attribuer à l'ESP32
uint8_t New_MAC_Address[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup() {
  Serial.begin(115200);  // Initialiser la communication série pour les messages de débogage

  // Configurer l'ESP32 en mode station (client Wi-Fi)
  WiFi.mode(WIFI_STA);

  // Modifier l'adresse MAC avant de démarrer la connexion Wi-Fi
  esp_err_t result = esp_wifi_set_mac(WIFI_IF_STA, New_MAC_Address);
  if (result == ESP_OK) {
    Serial.println("Adresse MAC modifiée avec succès.");
  } else {
    Serial.printf("Échec du changement de l'adresse MAC. Code d'erreur : %d\n", result);
  }

  // Configurer l'adresse IP statique avant d'établir la connexion Wi-Fi
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Échec de la configuration de l'IP statique.");
  }

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
}

void loop() {
  // Boucle principale où l'on peut ajouter des fonctionnalités supplémentaires
  // Exemple : mesurer des capteurs, envoyer des données sur le réseau, etc.
}