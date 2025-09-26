#include <Servo.h>

// Définition des pins
const int TRIG_PIN = 7;
const int ECHO_PIN = 6;
const int SERVO_PIN = 9;

// Déclaration du servomoteur
Servo hand;
const int ANGLE_DOWN = 100;   // main basse
const int ANGLE_UP   = 20;    // main levée

bool handUp = false;

// Lecture de la distance avec une moyenne
long readDistanceCm() {
  long sum = 0;
  const int N = 5; // moyenne sur 5 mesures
  for (int i = 0; i < N; i++) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000); 
    if (duration == 0) duration = 999 * 58;
    sum += duration / 58;
    delay(5);
  }
  return sum / N;
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  hand.attach(SERVO_PIN);
  hand.write(ANGLE_DOWN); 
}

void loop() {
  long distance = readDistanceCm();
  Serial.println(distance);

  if (!handUp && distance > 5 && distance < 15) {
    hand.write(ANGLE_UP);  
    handUp = true;
  } 
  else if (handUp && distance > 20) {
    hand.write(ANGLE_DOWN); 
    handUp = false;
  }

  delay(50);
}
