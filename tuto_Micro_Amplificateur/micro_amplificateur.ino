#define LED_WHITE 13
#define LED_YELLOW 16
#define LED_BLUE 23
#define LED_GREEN 14
#define LED_RED 22

#define MIC_PIN 34

void setup() {
  Serial.begin(115200);

  pinMode(LED_WHITE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_BLUE,   OUTPUT);
  pinMode(LED_GREEN,  OUTPUT);
  pinMode(LED_RED,    OUTPUT);
}

void loop() {
  int v = analogRead(MIC_PIN);
  Serial.println(v);

  // Seuil de bruit
  digitalWrite(LED_WHITE,  v > 2300);  
  digitalWrite(LED_YELLOW, v > 2600);  
  digitalWrite(LED_BLUE,   v > 2900);  
  digitalWrite(LED_GREEN,  v > 3300);  
  digitalWrite(LED_RED,    v > 3700);  

  delay(10);
}
