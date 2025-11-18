/*
 * Einfacher Servo-Test ohne BuggyControl-Bibliothek
 *
 * Bewegt einen Servo langsam von 0° bis 180° und zurück
 * Nutzt nur die grundlegende ESP32Servo-Bibliothek
 */

#include <ESP32Servo.h>

// Servo-Objekt erstellen
Servo myServo;

// Pin-Konfiguration
const int SERVO_PIN = 23;  // Pin für den Servo

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("========================================");
  Serial.println("     EINFACHER SERVO TEST");
  Serial.println("========================================");
  Serial.println();

  // Servo an Pin anhängen
  Serial.print("Initialisiere Servo an Pin ");
  Serial.println(SERVO_PIN);
  myServo.attach(SERVO_PIN);

  delay(500);
  Serial.println("Servo bereit!");
  Serial.println();
}

void loop() {
  Serial.println("--- Von 0° bis 180° ---");

  // Langsam von 0° bis 180° bewegen
  for (int angle = 0; angle <= 180; angle++) {
    myServo.write(angle);
    Serial.print("Winkel: ");
    Serial.print(angle);
    Serial.println("°");
    delay(50);  // 50ms pro Grad = ~9 Sekunden für volle Bewegung
  }

  Serial.println();
  delay(1000);  // 1 Sekunde Pause bei 180°

  Serial.println("--- Von 180° bis 0° ---");

  // Langsam von 180° bis 0° bewegen
  for (int angle = 180; angle >= 0; angle--) {
    myServo.write(angle);
    Serial.print("Winkel: ");
    Serial.print(angle);
    Serial.println("°");
    delay(50);  // 50ms pro Grad
  }

  Serial.println();
  delay(1000);  // 1 Sekunde Pause bei 0°
  Serial.println();
}
