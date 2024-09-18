
/*<--------------------------------------------------------------------------------------->*/
/*
    Ce code permet de communiquer avec le module LoRaWAN RA-07H en utilisant la connexion
    série. Il est possible d'envoyer des commandes AT au module et de recevoir les réponses

    C'est utile pour tester le module et pour comprendre son fonctionnement.

    Ce code est sous licence CC-BY-SA. 

    Informations du créateur du programme :
    Nom : Maxime Gillespie*
    Date de création : 2023-08-22
    Date de modification : 2023-08-29
    Version : 1.0
    
    *Assisté de : Chatgpt et Github Copilot
*/
/*<--------------------------------------------------------------------------------------->*/

//pin pour le module lorawan
int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;


void setup()
{
    // initialisation du module lorawan
    pin_init();

    // initialisation des ports série
    SerialUSB.begin(115200);
    Serial1.begin(115200);

    delay(1000);

    // afficher un message de bienvenue
    SerialUSB.println("Welcome to the LoRaWAN terminal.");
    SerialUSB.println();
}

// boucle principale
void loop()
{
    // vérifier si il y a des données sur le port série
    if (SerialUSB.available())
    {
        // lire les données du port série
        String data = SerialUSB.readStringUntil('\n');

        // afficher les données sur le port série
        SerialUSB.println(data);

        // envoyer les données au module lorawan
        Serial1.println(data);
    }

    // vérifier si il y a des données sur le port série du module lorawan
    if (Serial1.available())
    {
        // lire les données du port série du module lorawan
        String data = Serial1.readStringUntil('\n');

        // imprimer les données sur le port série
        SerialUSB.println(data);
    }
}

/*
    * Fonction pour initialiser les pins du module lorawan
*/
void pin_init()
{
    pinMode(PWR_KEY, OUTPUT);
    pinMode(RST_KEY, OUTPUT);
    pinMode(LOW_PWR_KEY, OUTPUT);

    digitalWrite(RST_KEY, LOW);
    digitalWrite(LOW_PWR_KEY, HIGH);
    digitalWrite(PWR_KEY, HIGH);

    digitalWrite(PWR_KEY, LOW);
    delay(3000);
    digitalWrite(PWR_KEY, HIGH);
    delay(10000);
}