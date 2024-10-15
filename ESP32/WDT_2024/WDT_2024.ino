#include <Arduino.h>
#include "esp_system.h"
#include "esp_task_wdt.h" // Inclure l'en-tête du Watchdog Timer (WDT) d'ESP-IDF pour gérer le Watchdog

/*
    Ce programme démontre comment configurer et utiliser le Watchdog Timer (WDT) sur un ESP32.
    Le Watchdog est un mécanisme de sécurité pour redémarrer l'ESP32 si la boucle principale ne répond pas dans un certain temps imparti.
    Ce programme est conçu à des fins éducatives pour illustrer la manipulation du Watchdog Timer.

    Informations du créateur du programme :
    Nom : Maxime Gillespie
    Date de création : 2024-xx-xx
    Version : 1.0
*/

// Variable externe permettant de désactiver le Watchdog de la tâche principale
extern bool loopTaskWDTEnabled;

void setup() {

  // Désactiver le Watchdog Timer pour la tâche principale
  loopTaskWDTEnabled = false;

  Serial.begin(115200);  // Initialiser la communication série pour le débogage
  delay(1000);  // Petit délai pour s'assurer que le moniteur série est prêt

  // Arrêter le Watchdog Timer si celui-ci est déjà initialisé
  esp_task_wdt_deinit();

  // Créer une structure de configuration pour le Watchdog Timer
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = 20000,  // Timeout en millisecondes (20 secondes)
    .idle_core_mask = 0,  // Appliquer le Watchdog à tous les cœurs du processeur
    .trigger_panic = true // Provoquer un "panic" et redémarrer sur timeout du WDT
  };

  // Initialiser le Watchdog Timer avec la configuration
  esp_task_wdt_init(&wdt_config);

  // Ajouter la tâche actuelle au Watchdog Timer
  esp_task_wdt_add(NULL);

  Serial.println("Watchdog Timer configuré avec un timeout de 20 secondes.");
}

void loop() {
  // Alimenter le Watchdog Timer pour éviter un redémarrage
  esp_task_wdt_reset();

  Serial.println("Watchdog Timer alimenté...");
  
  delay(25000);  // Délai de 25 secondes, plus long que le timeout WDT pour démontrer le comportement de réinitialisation
}

/*
    * La fonction setup() initialise le Watchdog Timer et configure la communication série.
    * Le Watchdog est désactivé au début et est réinitialisé avec de nouveaux paramètres.
    * Le WDT est utilisé pour surveiller la boucle principale. Si la fonction loop() ne s'exécute pas correctement, le WDT provoque un redémarrage de l'ESP32.

    * La fonction loop() est conçue pour échouer intentionnellement (délai plus long que le timeout du WDT) pour illustrer l'effet du WDT.
*/