/*<--------------------------------------------------------------------------------------->*/
/*
    Ce code permet de communiquer avec le module LoRaWAN RA-07H en utilisant la connexion
    série. Il est possible d'envoyer des commandes AT au module et de recevoir les réponses.

    Ce programme est utile pour tester le module et pour comprendre son fonctionnement.
    Il permet d'établir une communication bidirectionnelle entre le module et l'ordinateur.

    Ce code est sous licence CC-BY-SA. 

    Informations du créateur du programme :
    Nom : Maxime Gillespie*
    Date de création : 2023-08-22
    Date de modification : 2023-08-29
    Version : 1.0
    
    *Assisté de : ChatGPT et GitHub Copilot
*/
/*<--------------------------------------------------------------------------------------->*/

// Pins pour le module LoRaWAN
int PWR_KEY = 9;           // Pin pour activer/désactiver l'alimentation du module
int RST_KEY = 6;           // Pin pour réinitialiser le module
int LOW_PWR_KEY = 5;       // Pin pour activer/désactiver le mode basse consommation

void setup() {
    // Initialisation du module LoRaWAN
    pin_init();

    // Initialisation des ports série pour la communication avec le PC et le module LoRaWAN
    SerialUSB.begin(115200);   // Communication avec le PC pour affichage des messages de débogage
    Serial1.begin(115200);     // Communication avec le module RA-07H

    delay(1000);

    // Afficher un message de bienvenue
    SerialUSB.println("Bienvenue sur le terminal LoRaWAN.");
    SerialUSB.println();
}

// Boucle principale du programme
void loop() {
    // Vérifier s'il y a des données entrantes sur le port série USB (depuis le PC)
    if (SerialUSB.available()) {
        // Lire les données envoyées par l'utilisateur via le terminal
        String data = SerialUSB.readStringUntil('\n');

        // Afficher les données reçues sur le terminal USB (pour confirmation)
        SerialUSB.println("Commande reçue : " + data);

        // Envoyer les données au module LoRaWAN pour exécution
        Serial1.println(data);
    }

    // Vérifier s'il y a des données entrantes sur le port série du module LoRaWAN
    if (Serial1.available()) {
        // Lire la réponse envoyée par le module RA-07H
        String data = Serial1.readStringUntil('\n');

        // Imprimer la réponse du module sur le terminal USB
        SerialUSB.println("Réponse du module : " + data);
    }
}

/*
    * Fonction pour initialiser les pins du module LoRaWAN
    * Cette initialisation est essentielle pour mettre correctement sous tension et réinitialiser le module.
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
    delay(3000);  // Attendre un court instant avant de remettre sous tension
    digitalWrite(PWR_KEY, HIGH);
    delay(10000);  // Attendre que le module soit complètement initialisé
}
