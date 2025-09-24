#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Librairie pour l'accéléromètre MPU-9250 à installer via "Library Manager"
#include "MPU9250_asukiaaa.h"   

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

MPU9250_asukiaaa mySensor;

// Définition des axes
#define AXIS_X_SOURCE ay 
#define AXIS_Y_SOURCE ax 
#define AXIS_X_SIGN   -1  
#define AXIS_Y_SIGN    1  


float ballX = SCREEN_WIDTH / 2;
float ballY = SCREEN_HEIGHT / 2;
float velX = 0;
float velY = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA= GPIO21, SCL= GPIO22 sur l'ESP32

  // Initialisation de l'écran OLED 0,96 pouces
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erreur OLED");
    for (;;);
  }
  display.clearDisplay();
  display.display();

  // Initialisation de l'accéléromètre MPU-9250
  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();
}

void loop() {
  mySensor.accelUpdate();

  // Récupération des valeurs d'accélération
  float ax = mySensor.accelX();
  float ay = mySensor.accelY();

  // Application des réglages axes
  velX += (AXIS_X_SIGN * (float)(AXIS_X_SOURCE)) * 0.5;
  velY += (AXIS_Y_SIGN * (float)(AXIS_Y_SOURCE)) * 0.5;

  // Mise à jour de la position
  ballX += velX;
  ballY -= velY; 

  // Gestion des rebonds
  if (ballX <= 0 || ballX >= SCREEN_WIDTH - 1) {
    velX = -velX * 0.8;
    ballX = constrain(ballX, 0, SCREEN_WIDTH - 1);
  }
  if (ballY <= 0 || ballY >= SCREEN_HEIGHT - 1) {
    velY = -velY * 0.8;
    ballY = constrain(ballY, 0, SCREEN_HEIGHT - 1);
  }

  velX *= 0.95;
  velY *= 0.95;

  // Affichage
  display.clearDisplay();
  display.fillCircle((int)ballX, (int)ballY, 3, SSD1306_WHITE);
  display.display();

  delay(30);
}
