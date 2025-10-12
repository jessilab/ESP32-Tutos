#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Déclaration de l'écran OLED (adresse 0x3C pour la plupart des modules)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Colle ici le code généré par image2cpp
// Exemple : const unsigned char epd_bitmap_imageoled [] PROGMEM = { ... };

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1040)
const int epd_bitmap_allArray_LEN = 1;
const unsigned char* epd_bitmap_allArray[1] = {
	epd_bitmap_imageoled
};

void setup() {
  // Initialisation de l’écran
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Échec de l’initialisation de l’écran OLED"));
    for (;;);
  }
  display.clearDisplay();

  // Affiche l’image
  display.drawBitmap(0, 0, epd_bitmap_imageoled, 128, 64, SSD1306_WHITE);
  display.display();
}

void loop() {
  // Rien ici, l’image reste affichée
}
