/*<--------------------------------------------------------------------------------------->*/
/*
    Code de test pour envoyer un compteur de données via LoRaWAN.
    Ce programme est conçu pour démontrer l'envoi de données à travers un réseau LoRaWAN à l'aide du module RA-07H.
    Le compteur est envoyé périodiquement et la transmission est répétée en cas d'échec, dans la limite des tentatives prédéfinies.
*/
/*<--------------------------------------------------------------------------------------->*/

// Pins utilisés pour contrôler le module RA-07H
int PWR_KEY = 9;          // Pin pour activer/désactiver l'alimentation du module
int RST_KEY = 6;          // Pin pour réinitialiser le module
int LOW_PWR_KEY = 5;      // Pin pour activer/désactiver le mode basse consommation

// Variables pour gérer les envois et les éventuelles erreurs
int retryCount = 0;                   // Compteur de tentatives pour l'envoi de données
const int maxRetries = 3;             // Nombre maximum de tentatives d'envoi de paquets
bool ackReceived = false;             // Indicateur pour savoir si un accusé de réception (ACK) a été reçu

// Identifiants LoRaWAN modifiables à remplacer par les vôtres
String deveui = "6902955e6cf2c714";   // DEVEUI unique de l'appareil
String appeui = "0000000000000000";   // APPEUI de l'application
String appkey = "5fcce57182767d634ed74ec3e838a9fe";   // APPKEY pour l'authentification

// Compteur de données à envoyer via LoRaWAN
int counter = 0;                      // Valeur du compteur envoyé périodiquement

void setup() {
    pin_init();  // Initialisation des broches du module
    SerialUSB.begin(115200);   // Initialiser la communication avec le PC pour débogage
    Serial1.begin(115200);     // Initialiser la communication avec le module RA-07H

    delay(1000);
    SerialUSB.println("Initialisation du terminal LoRaWAN avec RA-07H...");

    configureModule();  // Configure le module LoRaWAN avec les paramètres nécessaires

    joinNetwork();  // Tente de rejoindre le réseau LoRaWAN
}

void loop() {
    // Si un accusé de réception est reçu ou si nous n'avons pas atteint le maximum de tentatives
    if (ackReceived || retryCount < maxRetries) {
        sendData(String(counter));  // Envoie la valeur actuelle du compteur
        counter++;  // Incrémenter le compteur à chaque envoi réussi ou tenté
    } else {
        SerialUSB.println("Nombre maximum de tentatives atteint, abandon de l'envoi.");
    }

    delay(60000);  // Attendre 60 secondes avant d'envoyer de nouvelles données
}

/*
    * Fonction pour configurer le module RA-07H avec les paramètres requis pour la connexion LoRaWAN
    * Cette fonction désactive ADR, ajuste la puissance de transmission et configure les identifiants OTAA.
*/
void configureModule() {
    SerialUSB.println("Configuration du module RA-07H...");

    // Désactiver ADR (Adaptative Data Rate) et configurer les autres paramètres de base
    sendATCommand("AT+CADR=0");  // Désactiver ADR
    sendATCommand("AT+CDATARATE=3");  // Fixer un débit de données élevé (DR3)
    sendATCommand("AT+CTXP=10");  // Configurer la puissance de transmission à 14 dBm

    // Configurer les identifiants OTAA (DEVEUI, APPEUI, APPKEY)
    sendATCommand("AT+CDEVEUI=" + deveui);  // Configurer le DEVEUI unique de l'appareil
    sendATCommand("AT+CAPPEUI=" + appeui);  // Configurer l'APPEUI de l'application
    sendATCommand("AT+CAPPKEY=" + appkey);  // Configurer l'APPKEY pour l'authentification

    sendATCommand("AT+CSAVE");  // Sauvegarder la configuration dans le module pour éviter de répéter la configuration à chaque redémarrage

    SerialUSB.println("Configuration terminée.");
}

/*
    * Fonction pour rejoindre le réseau LoRaWAN en utilisant OTAA (Over The Air Activation)
    * Une attente de 15 secondes est nécessaire pour laisser le temps au module de se connecter.
*/
void joinNetwork() {
    SerialUSB.println("Tentative d'adhésion au réseau LoRaWAN...");
    sendATCommand("AT+CJOIN=1,1,10,10");  // Rejoindre le réseau avec OTAA (essayer pendant 10 secondes)
    delay(15000);  // Attendre 15 secondes pour la jonction
}

/*
    * Fonction pour envoyer des données via le réseau LoRaWAN
    * L'envoi est effectué avec un payload comprenant la valeur actuelle du compteur
*/
void sendData(String payload) {
    SerialUSB.println("Envoi de données : " + payload);

    // Construire la commande d'envoi AT avec la taille du payload
    String command = "AT+DTRX=1,2," + String(payload.length()) + "," + payload;
    sendATCommand(command);

    // Attendre un accusé de réception (ACK) pendant 10 secondes
    delay(10000);

    // Vérifier la réponse pour voir si le paquet a été envoyé avec succès
    if (Serial1.available()) {
        String response = Serial1.readStringUntil('\n');
        SerialUSB.println(response);

        // Vérifier si un ACK ou une erreur a été reçu
        if (response.indexOf("OK+SENT") != -1) {
            SerialUSB.println("Données envoyées avec succès.");
            ackReceived = true;
            retryCount = 0;  // Réinitialiser le compteur de tentatives
        } else if (response.indexOf("ERR+SENT") != -1) {
            SerialUSB.println("Erreur de transmission, tentative de réessai...");
            retryCount++;
            if (retryCount < maxRetries) {
                delay(30000);  // Attendre 30 secondes avant de réessayer
                sendATCommand(command);  // Réessayer l'envoi du même paquet
            }
        }
    }
}

/*
    * Fonction pour envoyer une commande AT au module RA-07H et lire la réponse
    * Cette fonction envoie une commande AT et imprime la réponse du module sur le terminal de débogage
*/
void sendATCommand(String command) {
    Serial1.println(command);  // Envoyer la commande AT au module RA-07H
    delay(1500);  // Attendre une réponse du module

    // Lire et imprimer la réponse du module
    while (Serial1.available()) {
        String response = Serial1.readStringUntil('\n');
        SerialUSB.println(response);  // Afficher la réponse sur le terminal
    }
}

/*
    * Fonction pour initialiser les pins du module RA-07H
    * Cette initialisation est nécessaire pour contrôler correctement le module et le mettre sous tension.
*/
void pin_init() {
    pinMode(PWR_KEY, OUTPUT);  // Configurer la broche PWR_KEY comme sortie
    pinMode(RST_KEY, OUTPUT);  // Configurer la broche RST_KEY comme sortie
    pinMode(LOW_PWR_KEY, OUTPUT);  // Configurer la broche LOW_PWR_KEY comme sortie

    // Séquence d'initialisation pour mettre sous tension et réinitialiser le module
    digitalWrite(RST_KEY, LOW);  // Maintenir le module en état réinitialisé
    digitalWrite(LOW_PWR_KEY, HIGH);  // Désactiver le mode basse consommation
    digitalWrite(PWR_KEY, HIGH);  // Alimenter le module

    digitalWrite(PWR_KEY, LOW);  // Cycle de mise sous tension
    delay(3000);
    digitalWrite(PWR_KEY, HIGH);
    delay(10000);  // Attendre que le module soit complètement initialisé
}
