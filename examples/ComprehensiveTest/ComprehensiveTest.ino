#include <Arduino.h>
#include <BuggyControl.h>

/*
 * Umfassendes Testprogramm für die BuggyControl-Bibliothek
 *
 * Dieses Programm testet alle Funktionen der Bibliothek und
 * demonstriert verschiedene Fahrszenarien:
 * 1. Geradeausfahrt mit verschiedenen Geschwindigkeiten
 * 2. Kurvenfahrt mit Blinkern
 * 3. Rückwärtsfahrt mit Warnblinker
 * 4. Parkszenario
 * 5. Notbremsung
 */

// ========================================
// Hardware-Konfiguration
// ========================================

// Motor initialisieren (Pins 13 & 12 für H-Bridge)
Motor motor(13,      // pwm_pin_front
            12,      // pwm_pin_back
            14,      // high_pin_front
            27,      // high_pin_back
            100,     // max_duty (maximale Geschwindigkeit 100%)
            30,      // min_duty (minimale Geschwindigkeit 30%)
            500,     // direction_change_delay (0.5 Sekunden Pause beim Richtungswechsel)
            25000);  // freq (25kHz PWM-Frequenz)

// Servo initialisieren (Pin 23)
SteeringServo steering(23,   // control_pin
                       -3,   // power_pin
                       90,   // rest_position (90° = geradeaus)
                       20,   // max_steering_degree (±20° Lenkwinkel)
                       6);   // deadzone (Totzone für präzises Lenken)

// LED Manager initialisieren
LEDManager leftIndicator({16}, 0, 100, 1000);    // Blinker links (Pin 16)
LEDManager rightIndicator({5}, 0, 100, 1000);    // Blinker rechts (Pin 5)
LEDManager frontLights({19}, 1, 100, 1000);      // Frontlichter (Pin 19, standardmäßig an)
LEDManager rearLights({18}, 1, 100, 1000);       // Rücklichter (Pin 18, standardmäßig an)

// ========================================
// Hilfsfunktionen
// ========================================

void printHeader(const char* title) {
  Serial.println("\n========================================");
  Serial.println(title);
  Serial.println("========================================");
}

void printStatus(const char* action) {
  Serial.print("[STATUS] ");
  Serial.println(action);
}

// Warte bis Motor-Delay abgelaufen ist
void waitForMotor() {
  while (motor.checkDelay()) {  // Warte WÄHREND Delay aktiv ist
    delay(100);
  }
}

// ========================================
// Testszenarien
// ========================================

void scenario1_StraightDriving() {
  printHeader("SZENARIO 1: Geradeausfahrt");

  // Schritt 1: Langsame Beschleunigung
  printStatus("Langsame Beschleunigung auf 30%");
  steering.setRestPosition();
  frontLights.turnOn(100);
  motor.changeSpeedAbsolute(30);  // Absolut auf 30%
  delay(3000);

  // Schritt 2: Mittlere Geschwindigkeit
  printStatus("Beschleunigung auf 50%");
  motor.changeSpeedAbsolute(50);  // Absolut auf 50%
  delay(3000);

  // Schritt 3: Hohe Geschwindigkeit
  printStatus("Beschleunigung auf 80%");
  motor.changeSpeedAbsolute(80);  // Absolut auf 80%
  delay(3000);

  // Schritt 4: Allmähliches Abbremsen
  printStatus("Abbremsen auf 50%");
  motor.changeSpeedAbsolute(50);  // Absolut auf 50%
  delay(2000);

  printStatus("Abbremsen auf 30%");
  motor.changeSpeedAbsolute(30);  // Absolut auf 30%
  delay(2000);

  // Schritt 5: Stoppen
  printStatus("Stopp");
  motor.changeSpeedAbsolute(0);  // Absolut auf 0%
  rearLights.turnOn(100);  // Bremslichter
  delay(1000);
  rearLights.rest();       // Zurück zu normaler Helligkeit

  waitForMotor();
  delay(2000);
}

void scenario2_TurnLeft() {
  printHeader("SZENARIO 2: Linkskurve mit Blinker");

  // Blinker links setzen
  printStatus("Linker Blinker aktiviert");
  leftIndicator.startIndicating();
  delay(2000);

  // Beschleunigen
  printStatus("Beschleunigung auf 40%");
  motor.changeSpeedAbsolute(40);  // Absolut auf 40%
  delay(1000);

  // Lenken nach links
  printStatus("Lenke 50% nach links");
  steering.steerAbsolute(-50);
  delay(3000);

  // Geradeaus zurück
  printStatus("Zurueck in Mittelstellung");
  steering.setRestPosition();
  delay(1000);

  // Blinker aus
  printStatus("Blinker aus");
  leftIndicator.stopIndicating();

  // Stoppen
  printStatus("Stopp");
  motor.changeSpeedAbsolute(0);  // Absolut auf 0%
  waitForMotor();
  delay(2000);
}

void scenario3_TurnRight() {
  printHeader("SZENARIO 3: Rechtskurve mit Blinker");

  // Blinker rechts setzen
  printStatus("Rechter Blinker aktiviert");
  rightIndicator.startIndicating();
  delay(2000);

  // Beschleunigen
  printStatus("Beschleunigung auf 40%");
  motor.changeSpeedAbsolute(40);
  delay(1000);

  // Lenken nach rechts
  printStatus("Lenke 50% nach rechts");
  steering.steerAbsolute(50);
  delay(3000);

  // Geradeaus zurück
  printStatus("Zurueck in Mittelstellung");
  steering.setRestPosition();
  delay(1000);

  // Blinker aus
  printStatus("Blinker aus");
  rightIndicator.stopIndicating();

  // Stoppen
  printStatus("Stopp");
  motor.changeSpeedAbsolute(0);  // Absolut auf 0%
  waitForMotor();
  delay(2000);
}

void scenario4_Slalom() {
  printHeader("SZENARIO 4: Slalom-Fahrt");

  printStatus("Starte Slalom mit 40% Geschwindigkeit");
  motor.changeSpeedAbsolute(40);  // Absolut auf 40%
  delay(500);

  // Slalom-Sequenz
  for (int i = 0; i < 3; i++) {
    // Links
    printStatus("Kurve links");
    leftIndicator.startIndicating();
    steering.steerAbsolute(-60);
    delay(1500);
    leftIndicator.stopIndicating();

    // Mitte
    steering.setRestPosition();
    delay(500);

    // Rechts
    printStatus("Kurve rechts");
    rightIndicator.startIndicating();
    steering.steerAbsolute(60);
    delay(1500);
    rightIndicator.stopIndicating();

    // Mitte
    steering.setRestPosition();
    delay(500);
  }

  // Stoppen
  printStatus("Stopp nach Slalom");
  motor.changeSpeedAbsolute(0);  // Absolut auf 0%
  steering.setRestPosition();
  waitForMotor();
  delay(2000);
}

void scenario5_ReverseParking() {
  printHeader("SZENARIO 5: Rückwärts Einparken");

  // Warnblinker an
  printStatus("Warnblinker aktiviert");
  leftIndicator.startIndicating();
  rightIndicator.startIndicating();
  delay(2000);

  // Langsam rückwärts
  printStatus("Rueckwaertsfahrt -30%");
  motor.changeSpeedAbsolute(-30);  // Absolut auf -30%
  delay(2000);

  // Einlenken nach links beim Rückwärtsfahren
  printStatus("Einlenken nach links");
  steering.steerAbsolute(-40);
  delay(2000);

  // Geradeaus zurück
  printStatus("Lenkung geradeaus");
  steering.setRestPosition();
  delay(1000);

  // Stoppen
  printStatus("Stopp - Eingeparkt!");
  motor.changeSpeedAbsolute(0);  // Absolut auf 0%

  // Warnblinker aus
  leftIndicator.stopIndicating();
  rightIndicator.stopIndicating();

  waitForMotor();
  delay(2000);
}

void scenario6_EmergencyBrake() {
  printHeader("SZENARIO 6: Notbremsung");

  // Hohe Geschwindigkeit
  printStatus("Beschleunigung auf 70%");
  motor.changeSpeedAbsolute(70);  // Absolut auf 70%
  delay(3000);

  // Notbremsung!
  printStatus("!!! NOTBREMSUNG !!!");
  motor.changeSpeedAbsolute(0);  // Absolut auf 0%

  // Warnblinker und Bremslichter
  leftIndicator.startIndicating();
  rightIndicator.startIndicating();
  rearLights.turnOn(100);

  delay(3000);

  // Warnblinker aus
  leftIndicator.stopIndicating();
  rightIndicator.stopIndicating();
  rearLights.rest();

  waitForMotor();
  delay(2000);
}

void scenario7_NightDriving() {
  printHeader("SZENARIO 7: Nachtfahrt mit Lichtsteuerung");

  // Lichter einschalten
  printStatus("Scheinwerfer und Ruecklichter an");
  frontLights.turnOn(100);
  rearLights.turnOn(50);  // Rücklichter gedimmt
  delay(2000);

  // Langsame Fahrt
  printStatus("Langsame Fahrt (40%)");
  motor.changeSpeedAbsolute(40);  // Absolut auf 40%
  delay(4000);

  // Kurve mit Blinker
  printStatus("Rechtsabbiegen mit Blinker");
  rightIndicator.startIndicating();
  steering.steerAbsolute(50);
  delay(3000);
  rightIndicator.stopIndicating();
  steering.setRestPosition();

  // Stoppen
  printStatus("Anhalten");
  motor.changeSpeedAbsolute(0);  // Absolut auf 0%

  // Lichter zurück auf Standard
  frontLights.rest();
  rearLights.rest();

  waitForMotor();
  delay(2000);
}

void scenario8_SpeedTest() {
  printHeader("SZENARIO 8: Geschwindigkeitstest");

  printStatus("Teste verschiedene Geschwindigkeitsstufen");
  steering.setRestPosition();

  int speeds[] = {20, 40, 60, 80, 100};  // Absolute Werte: 20%, 40%, 60%, 80%, 100%

  for (int i = 0; i < 5; i++) {
    char msg[50];
    sprintf(msg, "Setze auf %d%% absolut", speeds[i]);
    printStatus(msg);

    motor.changeSpeedAbsolute(speeds[i]);
    Serial.print("  Neuer Duty: ");
    Serial.println(motor.getCurrentDuty());
    delay(2000);
  }

  // Stoppen
  printStatus("Geschwindigkeitstest abgeschlossen - Stopp");
  motor.changeSpeedAbsolute(0);  // Absolut auf 0%
  waitForMotor();
  delay(2000);
}

void scenario9_SteeringTest() {
  printHeader("SZENARIO 9: Lenkungstest");

  printStatus("Teste vollstaendigen Lenkbereich");

  int angles[] = {-100, -75, -50, -25, 0, 25, 50, 75, 100};

  for (int i = 0; i < 9; i++) {
    char msg[50];
    sprintf(msg, "Lenkwinkel: %d%%", angles[i]);
    printStatus(msg);

    steering.steerAbsolute(angles[i]);
    Serial.print("  Aktueller Servowinkel: ");
    Serial.println(steering.getCurrentSteeringDegree());
    delay(1000);
  }

  // Zurück zur Mitte
  printStatus("Zurueck zur Mittelstellung");
  steering.setRestPosition();
  delay(1000);
}

void scenario10_LEDTest() {
  printHeader("SZENARIO 10: LED-Funktionstest");

  // Test 1: Einzelne LEDs
  printStatus("Test 1: Frontlichter");
  frontLights.turnOn(100);
  delay(2000);
  frontLights.turnOff();
  delay(1000);

  printStatus("Test 2: Ruecklichter");
  rearLights.turnOn(100);
  delay(2000);
  rearLights.turnOff();
  delay(1000);

  // Test 2: Blinker nacheinander
  printStatus("Test 3: Linker Blinker");
  leftIndicator.startIndicating();
  delay(3000);
  leftIndicator.stopIndicating();
  delay(1000);

  printStatus("Test 4: Rechter Blinker");
  rightIndicator.startIndicating();
  delay(3000);
  rightIndicator.stopIndicating();
  delay(1000);

  // Test 3: Warnblinker
  printStatus("Test 5: Warnblinker");
  leftIndicator.startIndicating();
  rightIndicator.startIndicating();
  delay(4000);
  leftIndicator.stopIndicating();
  rightIndicator.stopIndicating();
  delay(1000);

  // Test 4: Alle LEDs gleichzeitig
  printStatus("Test 6: Alle LEDs an");
  frontLights.turnOn(100);
  rearLights.turnOn(100);
  leftIndicator.turnOn(100);
  rightIndicator.turnOn(100);
  delay(3000);

  printStatus("Test 7: Alle LEDs aus");
  frontLights.turnOff();
  rearLights.turnOff();
  leftIndicator.turnOff();
  rightIndicator.turnOff();
  delay(2000);

  // Test 5: Blink-Geschwindigkeiten
  printStatus("Test 8: Schneller Blinker (300ms)");
  leftIndicator.setIndicatorTiming(300);
  leftIndicator.startIndicating();
  delay(3000);
  leftIndicator.stopIndicating();
  delay(1000);

  printStatus("Test 9: Langsamer Blinker (1000ms)");
  leftIndicator.setIndicatorTiming(1000);
  leftIndicator.startIndicating();
  delay(4000);
  leftIndicator.stopIndicating();

  // Zurück zu Standard-Timing
  leftIndicator.setIndicatorTiming(500);

  // Alle LEDs in Ruhezustand
  printStatus("Alle LEDs in Ruhezustand");
  frontLights.rest();
  rearLights.rest();
  leftIndicator.rest();
  rightIndicator.rest();
  delay(2000);
}

// ========================================
// Setup und Loop
// ========================================

void setup() {
  Serial.begin(115200);
  delay(2000);  // Zeit für Serial Monitor

  Serial.println("\n\n");
  Serial.println("########################################");
  Serial.println("#     BUGGYCONTROL TEST SUITE         #");
  Serial.println("#  Umfassendes Testprogramm mit Logik #");
  Serial.println("########################################");
  Serial.println();
  Serial.println("Initialisiere Hardware...");

  // Pins als Output setzen
  pinMode(13, OUTPUT);  // Motor vorwärts
  pinMode(12, OUTPUT);  // Motor rückwärts
  pinMode(16, OUTPUT);  // Blinker links
  pinMode(19, OUTPUT);  // Frontlicht
  pinMode(5, OUTPUT);   // Blinker rechts
  pinMode(18, OUTPUT);  // Rücklicht
  pinMode(23, OUTPUT);  // Servo

  Serial.println("Hardware initialisiert!");
  Serial.println();

  // Motor-Konfiguration anpassen
  printStatus("Konfiguriere Motor-Parameter");
  motor.setDeadzone(5);        // Deadzone: ±5%
  motor.setThreshold(30);      // Threshold für Fading: 30%
  motor.setThresholdTime(1500); // Fading-Zeit: 1.5 Sekunden

  Serial.print("  - Motor Front Pin: ");
  Serial.println(motor.getPin(0));
  Serial.print("  - Motor Back Pin: ");
  Serial.println(motor.getPin(1));
  Serial.print("  - Initial Duty: ");
  Serial.println(motor.getCurrentDuty());
  Serial.print("  - Initial Steering: ");
  Serial.println(steering.getCurrentSteeringDegree());

  delay(2000);

  // ========================================
  // Führe alle Testszenarien aus
  // ========================================

  scenario1_StraightDriving();
  scenario2_TurnLeft();
  scenario3_TurnRight();
  scenario4_Slalom();
  scenario5_ReverseParking();
  scenario6_EmergencyBrake();
  scenario7_NightDriving();
  scenario8_SpeedTest();
  scenario9_SteeringTest();
  scenario10_LEDTest();

  // Abschlussmeldung
  Serial.println("\n\n");
  Serial.println("########################################");
  Serial.println("#    ALLE TESTS ABGESCHLOSSEN!        #");
  Serial.println("########################################");
  Serial.println();
  Serial.println("Alle Szenarien wurden erfolgreich getestet.");
  Serial.println("Der Buggy befindet sich im Ruhezustand.");
  Serial.println();
  Serial.println("Bereit für manuelle Steuerung in loop().");
  Serial.println("========================================");
}

void loop() {
  // Hier kann deine eigene Steuerungslogik implementiert werden
  // Zum Beispiel: Bluetooth-Steuerung, Sensoreingaben, etc.

  delay(1000);
}
