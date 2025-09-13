#include "bitmaps.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Définition de la taille de l'écran
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Déclaration des bitmaps
const unsigned char* happy_frames[] = {
  epd_bitmap_frame1_happy,
  epd_bitmap_frame2_happy,
  epd_bitmap_frame3_happy,
  epd_bitmap_frame4_happy,
  epd_bitmap_frame5_happy
};

const unsigned char* angry_frames[] = {
  epd_bitmap_frame1_angry,
  epd_bitmap_frame2_angry,
  epd_bitmap_frame3_angry,
  epd_bitmap_frame4_angry,
  epd_bitmap_frame5_angry
};

// Leds
const int ledRouge = 25;
const int ledVerte = 26;

// capteur ultrason
const int trigPin = 12;
const int echoPin = 14;

long duration;
int distance;

// Affiche l'animation
void afficherAnimation(const unsigned char* frames[], int nombreFrames, int w, int h) {
  for (int i = 0; i < nombreFrames; i++) {
    display.clearDisplay();
    display.drawBitmap(0, 0, frames[i], w, h, 1);
    display.display();
    delay(150);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(ledRouge, OUTPUT);
  pinMode(ledVerte, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
}

void loop() {
  // Mesure de la distance avec le capteur
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000UL);
  distance = (duration == 0) ? -1 : duration * 0.034 / 2;

  if (distance > 0 && distance < 10) {
    // Si un objet est proche, animation du personnage énervé
    digitalWrite(ledRouge, HIGH);
    digitalWrite(ledVerte, LOW);
    afficherAnimation(angry_frames, sizeof(angry_frames)/sizeof(angry_frames[0]), 128, 64);

  } else if (distance >= 10) {
    // Si pas d'ibjet proche, animation du personnage heureux
    digitalWrite(ledRouge, LOW);
    digitalWrite(ledVerte, HIGH);
    afficherAnimation(happy_frames, sizeof(happy_frames)/sizeof(happy_frames[0]), 128, 64);

  } else {
    // Si pas de mesure
    digitalWrite(ledRouge, LOW);
    digitalWrite(ledVerte, LOW);
    afficherAnimation(happy_frames, sizeof(happy_frames)/sizeof(happy_frames[0]), 128, 64);
    delay(200);
  }
}
