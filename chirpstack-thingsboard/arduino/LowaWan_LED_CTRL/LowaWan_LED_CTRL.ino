/*<--------------------------------------------------------------------------------------->*/
/*
    Code Minimal pour Configurer le Module RA-07H, Rejoindre le Réseau LoRaWAN,
    et Contrôler une LED Basée sur les Messages Reçus ("AA" pour Allumer, "BB" pour Éteindre).
    Format des Messages Reçus : OK+RECV:00,01,01,AA
    Confirmation de Jonction : OK+RECV:02,00,00
    Prompt à Ignorer : ASR6501:~#
*/
/*<--------------------------------------------------------------------------------------->*/

#include <Arduino.h>

// Définition des pins pour contrôler le module RA-07H
const int PWR_KEY = 8;        // Pin 8 pour PWR_KEY
const int RST_KEY = 6;        // Pin 6 pour RST_KEY
const int LOW_PWR_KEY = 5;    // Pin 5 pour LOW_PWR_KEY

// Définition de la pin pour la LED
const int LED_PIN = 9;        // Pin 9 pour la LED

// Identifiants LoRaWAN
const String deveui = "7d8a412c0c2289f1";                // Remplacer par votre DEVEUI
const String appeui = "0000000000000000";                // Remplacer par votre APPEUI
const String appkey = "da9cb3f9c8026a8ee0e1198346b51404"; // Remplacer par votre APPKEY

// Variables pour gérer la réception des données
String incomingMessage = "";   // Buffer pour le message reçu

void setup() {
    // Initialiser les pins de contrôle
    pin_init();
    
    // Initialiser la pin LED comme sortie et éteindre la LED au démarrage
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW); // Éteindre la LED au démarrage

    // Initialiser les communications série
    SerialUSB.begin(115200);   // Communication avec PC
    while (!SerialUSB) {
        ; // Attendre que la connexion USB soit établie
    }
    SerialUSB.println("Initialisation du terminal LoRaWAN avec RA-07H...");

    Serial1.begin(115200);     // Communication avec le module RA-07H
    SerialUSB.println("Communication série initialisée à 115200 baud.");

    delay(1000); // Attendre un court instant avant la configuration

    configureModule();  // Configurer le module LoRaWAN avec les commandes AT personnalisées

    joinNetwork();  // Tenter de rejoindre le réseau LoRaWAN
}

void loop() {
    // Vérifier s'il y a des données disponibles sur Serial1 (du module RA-07H)
    while (Serial1.available()) {
        char c = Serial1.read(); // Lire un caractère
        incomingMessage += c;    // Ajouter au buffer du message reçu

        // Si le caractère est un saut de ligne, traiter le message
        if (c == '\n') {
            processIncomingMessage(incomingMessage);
            incomingMessage = ""; // Réinitialiser le buffer
        }
    }

    // Petite pause pour éviter une utilisation excessive du CPU
    delay(10);
}

/*
    * Fonction pour initialiser les pins de contrôle du module RA-07H
*/
void pin_init() {
    pinMode(PWR_KEY, OUTPUT);
    pinMode(RST_KEY, OUTPUT);
    pinMode(LOW_PWR_KEY, OUTPUT);

    // Initialiser les pins pour mettre le module sous tension
    digitalWrite(RST_KEY, LOW);
    digitalWrite(LOW_PWR_KEY, HIGH);
    digitalWrite(PWR_KEY, HIGH);

    // Effectuer une réinitialisation matérielle du module
    digitalWrite(PWR_KEY, LOW);
    delay(3000);
    digitalWrite(PWR_KEY, HIGH);
    delay(10000);  // Attendre que le module soit initialisé
}

/*
    * Fonction pour envoyer une commande AT au module RA-07H et lire la réponse
*/
void sendATCommand(String command, unsigned long timeout = 2000) {
    SerialUSB.print("Envoi de la commande AT: ");
    SerialUSB.println(command);
    
    Serial1.println(command);  // Envoyer la commande au module

    unsigned long startTime = millis();
    String response = "";

    // Attendre une réponse ou un timeout
    while (millis() - startTime < timeout) {
        if (Serial1.available()) {
            char c = Serial1.read();
            response += c;
            // Echo uniquement si ce n'est pas le prompt
            if (!isPromptCharacter(c)) {
                SerialUSB.write(c); // Echo de la réponse dans le Moniteur Série
            }
        }
    }

    SerialUSB.println(); // Nouvelle ligne pour la clarté
}

/*
    * Fonction pour vérifier si un caractère fait partie du prompt
*/
bool isPromptCharacter(char c) {
    // Définir les caractères du prompt à ignorer
    const char prompt[] = "ASR6501:~# ";
    static int promptIndex = 0;
    static int promptLength = sizeof(prompt) - 1; // Exclure le caractère nul

    if (c == prompt[promptIndex]) {
        promptIndex++;
        if (promptIndex >= promptLength) {
            promptIndex = 0; // Réinitialiser une fois le prompt détecté
            return true;      // Caractère faisant partie du prompt
        }
        return true;
    }
    else {
        promptIndex = 0; // Réinitialiser si la séquence est brisée
        return false;     // Caractère normal
    }
}

/*
    * Fonction pour configurer le module RA-07H avec les commandes AT personnalisées
*/
void configureModule() {
    SerialUSB.println("Configuration du module RA-07H...");

    // Restaurer les paramètres d'usine
    sendATCommand("AT+CRESTORE");
    delay(5000);  // Attendre que le module redémarre

    // Configurer les paramètres spécifiques
    sendATCommand("AT+CADR=1");                // Activer ADR (Adaptive Data Rate)
    sendATCommand("AT+CFREQBANDMASK=0002");     // Définir le masque de bande de fréquence
    sendATCommand("AT+CTXP=5");                 // Définir la puissance de transmission

    // Configurer les identifiants LoRaWAN
    sendATCommand("AT+CDEVEUI=" + deveui);      // Définir le DEVEUI
    sendATCommand("AT+CAPPEUI=" + appeui);      // Définir l'APPEUI
    sendATCommand("AT+CAPPKEY=" + appkey);      // Définir l'APPKEY

    // Sauvegarder la configuration
    sendATCommand("AT+CSAVE");

    SerialUSB.println("Configuration terminée.");
    delay(2000); // Attendre un court instant pour s'assurer que les commandes sont traitées
}

/*
    * Fonction pour tenter de rejoindre le réseau LoRaWAN et attendre la confirmation spécifique
*/
void joinNetwork() {
    SerialUSB.println("Tentative d'adhésion au réseau LoRaWAN...");
    
    // Envoyer la commande de jonction avec les paramètres spécifiés
    // AT+CJOIN=<mode>,<retries>,<timeout>,<dr>
    // Exemple: AT+CJOIN=1,0,10,8
    sendATCommand("AT+CJOIN=1,0,10,8");

    // Attendre la réponse spécifique pour confirmer la jonction
    unsigned long startTime = millis();
    bool joined = false;
    String response = "";

    while (millis() - startTime < 20000) { // Attendre jusqu'à 20 secondes
        if (Serial1.available()) {
            char c = Serial1.read();
            response += c;
            // Echo uniquement si ce n'est pas le prompt
            if (!isPromptCharacter(c)) {
                SerialUSB.write(c); // Echo de la réponse dans le Moniteur Série
            }

            // Vérifier si la réponse contient "OK+RECV:02,00,00"
            if (response.indexOf("OK+RECV:02,00,00") != -1) {
                SerialUSB.println("\nAdhésion réussie au réseau LoRaWAN.");
                joined = true;
                break;
            }

            // Vérifier si la réponse contient "ERROR"
            if (response.indexOf("ERROR") != -1) {
                SerialUSB.println("\nÉchec de l'adhésion au réseau LoRaWAN.");
                break;
            }
        }
    }

    if (!joined) {
        SerialUSB.println("La tentative d'adhésion a expiré ou a échoué.");
    }
}

/*
    * Fonction pour traiter les messages reçus et contrôler la LED
*/
void processIncomingMessage(String message) {
    // Vérifier si le message est le prompt et l'ignorer
    if (message.startsWith("ASR6501:~#")) {
        // Ignorer le prompt
        return;
    }

    SerialUSB.println("Message Reçu: " + message);

    // Nettoyer le message en supprimant les espaces et les sauts de ligne
    message.trim();

    // Format du message attendu : OK+RECV:00,01,01,AA
    // Nous devons extraire la dernière partie après la dernière virgule
    int lastCommaIndex = message.lastIndexOf(',');
    if (lastCommaIndex != -1 && lastCommaIndex + 1 < message.length()) {
        String payload = message.substring(lastCommaIndex + 1); // Extraire le payload
        payload.trim(); // Supprimer les espaces supplémentaires

        SerialUSB.println("Payload Extrait: " + payload);

        if (payload.equals("AA")) {
            digitalWrite(LED_PIN, HIGH); // Allumer la LED
            SerialUSB.println("LED Allumée.");
        }
        else if (payload.equals("BB")) {
            digitalWrite(LED_PIN, LOW); // Éteindre la LED
            SerialUSB.println("LED Éteinte.");
        }
        else {
            SerialUSB.println("Payload Inconnu. Aucune Action Entreprise.");
        }
    }
    else {
        SerialUSB.println("Message Malformé. Impossible d'Extraire le Payload.");
    }
}
