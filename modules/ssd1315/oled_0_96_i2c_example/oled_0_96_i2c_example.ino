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