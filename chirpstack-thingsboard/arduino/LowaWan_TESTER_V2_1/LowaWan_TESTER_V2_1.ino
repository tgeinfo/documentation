/*<--------------------------------------------------------------------------------------->*/
/*
    Code de test pour envoyer un compteur de données via LoRaWAN.
*/
/*<--------------------------------------------------------------------------------------->*/

int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;

// Variables pour gérer les envois et les erreurs
int retryCount = 0;
const int maxRetries = 3;  // Maximum de tentatives pour l'envoi de paquets
bool ackReceived = false;

// Identifiants LoRaWAN modifiables
String deveui = "6902955e6cf2c714";  // Remplacer par ton DEVEUI
String appeui = "0000000000000000";  // Remplacer par ton APPEUI
String appkey = "5fcce57182767d634ed74ec3e838a9fe";  // Remplacer par ton APPKEY

// Compteur à envoyer via LoRaWAN
int counter = 0;

void setup() {
    pin_init();
    SerialUSB.begin(115200);   // Communication avec PC
    Serial1.begin(115200);     // Communication avec RA-07H module

    delay(1000);
    SerialUSB.println("Initialisation du terminal LoRaWAN avec RA-07H...");

    configureModule();  // Configure le module LoRaWAN

    joinNetwork();  // Tente de rejoindre le réseau
}

void loop() {
    if (ackReceived || retryCount < maxRetries) {
        // Incrémenter le compteur et l'envoyer
        sendData(String(counter));
        counter++;  // Incrémente le compteur à chaque envoi
    } else {
        SerialUSB.println("Nombre maximum de tentatives atteint, abandon de l'envoi.");
    }

    delay(60000);  // Attendre 60 secondes avant d'envoyer de nouvelles données
}

/*
    * Fonction pour configurer le module RA-07H
*/
void configureModule() {
    SerialUSB.println("Configuration du module RA-07H...");

    // Désactiver ADR et configurer les autres paramètres de base
    sendATCommand("AT+CADR=0");  // Désactiver ADR
    sendATCommand("AT+CDATARATE=3");  
    sendATCommand("AT+CTXP=10");  // Puissance de transmission à 14 dBm

    // Configurer les identifiants OTAA (DEVEUI, APPEUI, APPKEY)
    sendATCommand("AT+CDEVEUI=" + deveui);  // Configurer le DEVEUI
    sendATCommand("AT+CAPPEUI=" + appeui);  // Configurer l'APPEUI
    sendATCommand("AT+CAPPKEY=" + appkey);  // Configurer l'APPKEY

    sendATCommand("AT+CSAVE");  // Sauvegarder la configuration

    SerialUSB.println("Configuration terminée.");
}

/*
    * Fonction pour joindre le réseau LoRaWAN
*/
void joinNetwork() {
    SerialUSB.println("Tentative d'adhésion au réseau LoRaWAN...");
    sendATCommand("AT+CJOIN=1,1,10,10");
    delay(15000);  // Attendre 15 secondes pour la jonction
}

/*
    * Fonction pour envoyer des données au réseau LoRaWAN
*/
void sendData(String payload) {
    SerialUSB.println("Envoi de données : " + payload);

    // Construire la commande d'envoi AT
    String command = "AT+DTRX=1,2," + String(payload.length()) + "," + payload;
    sendATCommand(command);

    // Attendre un accusé de réception (ACK) pendant 10 secondes
    delay(10000);

    // Vérifier la réponse pour voir si le paquet a été envoyé avec succès
    if (Serial1.available()) {
        String response = Serial1.readStringUntil('\n');
        SerialUSB.println(response);

        // Vérifier si un ACK ou une erreur a été reçue
        if (response.indexOf("OK+SENT") != -1) {
            SerialUSB.println("Données envoyées avec succès.");
            ackReceived = true;
            retryCount = 0;  // Réinitialiser le compteur de tentatives
        } else if (response.indexOf("ERR+SENT") != -1) {
            SerialUSB.println("Erreur de transmission, tentative de réessai...");
            retryCount++;
            if (retryCount < maxRetries) {
                delay(30000);  // Attendre 30 secondes avant de réessayer
                sendATCommand(command);  // Réessayer l'envoi
            }
        }
    }
}

/*
    * Fonction pour envoyer une commande AT au module RA-07H et lire la réponse
*/
void sendATCommand(String command) {
    Serial1.println(command);
    delay(1500);  // Attendre une réponse du module

    // Lire la réponse du module
    while (Serial1.available()) {
        String response = Serial1.readStringUntil('\n');
        SerialUSB.println(response);  // Afficher la réponse sur le SerialUSB
    }
}

/*
    * Fonction pour initialiser les pins du module RA-07H
*/
void pin_init() {
    pinMode(PWR_KEY, OUTPUT);
    pinMode(RST_KEY, OUTPUT);
    pinMode(LOW_PWR_KEY, OUTPUT);

    digitalWrite(RST_KEY, LOW);
    digitalWrite(LOW_PWR_KEY, HIGH);
    digitalWrite(PWR_KEY, HIGH);

    digitalWrite(PWR_KEY, LOW);
    delay(3000);
    digitalWrite(PWR_KEY, HIGH);
    delay(10000);  // Attendre que le module soit initialisé
}
