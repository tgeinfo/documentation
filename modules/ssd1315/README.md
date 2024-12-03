# Guide d'utilisation du SSD1315 avec un Arduino Uno via I2C

Ce guide vous montre comment utiliser un écran OLED SSD1315 avec un Arduino Uno en utilisant les bibliothèques **Adafruit_SSD1306** et **Adafruit_GFX**. Vous trouverez ci-dessous le matériel nécessaire, les connexions, la configuration logicielle et un exemple de code pour commencer.

## Matériel nécessaire

- Un module OLED SSD1315 (compatible I2C)
- Arduino Uno
- Fils de connexion (Dupont)

## Connexions

> Note : Sur l'Arduino Uno, les broches SDA et A4, ainsi que SCL et A5, sont les mêmes entrées/sorties utilisées pour la communication I2C.

1. **VCC** du module OLED vers **5V** de l'Arduino.
2. **GND** du module OLED vers **GND** de l'Arduino.
3. **SCL (CLK)** du module OLED vers **A5** de l'Arduino (SCL).
4. **SDA (DIN)** du module OLED vers **A4** de l'Arduino (SDA).

## Configuration logicielle

1. **Installer les bibliothèques**
   - Ouvrez l'**Arduino IDE**.
   - Allez dans **Outils** → **Gérer les bibliothèques...**.
   - Recherchez **Adafruit SSD1306** et **Adafruit GFX**, puis installez-les.

## Code d'exemple

Voici un exemple de code pour initialiser l'écran et afficher du texte pour valider son fonctionnement.

```cpp
// Exemple de code pour l'utilisation d'un écran OLED SSD1315 avec un Arduino Uno
// Créé par Maxime

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // Largeur de l'écran OLED, en pixels
#define SCREEN_HEIGHT 64 // Hauteur de l'écran OLED, en pixels

// Déclaration pour un écran SSD1306 connecté en I2C (broches SDA, SCL)
#define OLED_RESET     -1 // Broche de réinitialisation (ou -1 si partagée avec la réinitialisation de l'Arduino)
#define SCREEN_ADDRESS 0x3D // Adresse I2C de l'écran (0x3D pour 128x64, 0x3C pour 128x32)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = génère la tension d'affichage à partir de 3.3V en interne
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Échec de l'allocation de l'écran SSD1306"));
    for(;;); // Ne pas continuer, boucle infinie
  }

  // Afficher le contenu initial du tampon sur l'écran (splash screen Adafruit)
  display.display();
  delay(2000); // Pause de 2 secondes

  // Effacer le tampon
  display.clearDisplay();

  // Afficher un message pour valider le fonctionnement
  display.setTextSize(1);             // Taille du texte
  display.setTextColor(SSD1306_WHITE); // Couleur du texte
  display.setCursor(0, 1);           // Position du texte
  display.println("Bonjour le monde!");
  display.println("TSO ROCK");

  // Afficher le tampon sur l'écran
  display.display();
  delay(2000);
}

void loop() {
  // Pas besoin de répéter quoi que ce soit pour cet exemple simple
}
```

## Explications

1. **Bibliothèques** : Les bibliothèques **Wire**, **Adafruit_GFX**, et **Adafruit_SSD1306** sont nécessaires pour contrôler l'écran.
2. **Initialisation** : `display.begin()` initialise l'écran SSD1315 avec l'adresse I2C (par défaut 0x3C ou 0x3D selon les modules).
3. **Effacer et afficher** : `clearDisplay()` efface l'écran avant chaque utilisation, et `display.display()` envoie le contenu à l'écran.
4. **Texte** : `setTextSize()`, `setTextColor()`, et `setCursor()` permettent de personnaliser le texte.

## Astuce

Vérifiez l'adresse I2C de votre module SSD1315 en utilisant un scanner I2C. Cela peut être utile si l'écran ne fonctionne pas correctement.

## Références

- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)

## Note pour les étudiants

Pour le projet de stationnement, veuillez vous référer à la documentation suivante pour plus de détails sur l'utilisation du SSD1315 : [Documentation SSD1315](https://www.waveshare.com/wiki/0.96inch_OLED_Module).

**Important : Assurez-vous de retirer la résistance R1 pour utiliser l'écran en mode I2C.**

## Conclusion

Avec ce guide, vous devriez être en mesure d'afficher du texte sur votre écran OLED SSD1315 en utilisant un Arduino Uno. Pour des applications plus complexes, explorez d'autres fonctions de la bibliothèque **Adafruit_GFX** comme l'affichage de formes ou de graphismes.

