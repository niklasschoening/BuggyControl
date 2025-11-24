#include <Arduino.h>
#include <BuggyControl.h>

// Umfassendes Testprogramm für alle BuggyControl-Funktionen

// Motor initialisieren
Motor motor(13,      // pwm_pin_front
            12,      // pwm_pin_back
            14,      // high_pin_front (Enable Pin)
            27,      // high_pin_back (Enable Pin)
            100,     // max_duty
            30,      // min_duty
            1000,    // direction_change_delay (ms)
            100000);  // freq (Hz)

// Servo initialisieren
SteeringServo steering(23,   // control_pin
                       -1,   // power_pin (kein Power-Pin)
                       90,   // rest_position
                       70,   // max_steering_degree
                       6);   // deadzone

// LEDs initialisieren ---- {Led-Pins}, {Led-Channel}, rest-state, brightness, pwm-Frequenz
LEDManager leftIndicator({16}, {6}, 0, 100, 1000);    // Pin 16, aus, 100% brightness, 1kHz
LEDManager rightIndicator({5}, {7}, 0, 100, 1000);    // Pin 5, aus, 100% brightness, 1kHz
LEDManager frontLights({19}, {12}, 1, 100, 1000);      // Pin 19, an, 100% brightness, 1kHz
LEDManager rearLights({18}, {13}, 1, 100, 1000);       // Pin 18, an, 100% brightness, 1kHz

void printTestHeader(const char* testName) {
  Serial.println("\n========================================");
  Serial.print("TEST: ");
  Serial.println(testName);
  Serial.println("========================================");
}

void testMotorFunctions() {
  printTestHeader("Motor-Klasse");

  // Test 1: getPin()
  Serial.println("\n[1] Teste getPin()");
  Serial.print("  - Front Pin: ");
  Serial.println(motor.getPin(1));
  Serial.print("  - Back Pin: ");
  Serial.println(motor.getPin(-1));

  // Test 2: getCurrentDuty()
  Serial.println("\n[2] Teste getCurrentDuty() - Initial");
  Serial.print("  - Aktueller Duty: ");
  Serial.println(motor.getCurrentDuty());

  // Test 3: changeSpeed() - Vorwärts
  Serial.println("\n[3] Teste changeSpeed() - Erhoehe um 30%");
  motor.changeSpeedAbsolute(50);  // Von 0 auf 30
  delay(500);
  Serial.print("  - Duty nach Aenderung: ");
  Serial.println(motor.getCurrentDuty());
  delay(2000);

  // Test 4: changeSpeed() - Höhere Geschwindigkeit
  Serial.println("\n[4] Teste changeSpeed() - Erhoehe um weitere 40% (auf 70%)");
  motor.changeSpeedAbsolute(70);  // Von 30 auf 70
  delay(500);
  Serial.print("  - Duty nach Aenderung: ");
  Serial.println(motor.getCurrentDuty());
  delay(2000);

  // Test 5: changeSpeed() - Reduzieren
  Serial.println("\n[5] Teste changeSpeed() - Reduziere um 70% (auf 0)");
  motor.changeSpeedAbsolute(0);  // Von 70 auf 0
  delay(1500);
  Serial.print("  - Duty nach Stopp: ");
  Serial.println(motor.getCurrentDuty());

  // Test 6: changeSpeed() - Rückwärts
  Serial.println("\n[6] Teste changeSpeed() - Rueckwaerts um -50%");
  motor.changeSpeedAbsolute(-50);  // Von 0 auf -50
  delay(500);
  Serial.print("  - Duty nach Änderung: ");
  Serial.println(motor.getCurrentDuty());
  delay(2000);

  // Test 7: changeSpeed() - Zurück zu 0
  Serial.println("\n[7] Teste changeSpeed() - Erhöhe um 50% (zurück zu 0)");
  motor.changeSpeedAbsolute(0);  // Von -50 auf 0
  delay(1500);
  Serial.print("  - Duty nach Stopp: ");
  Serial.println(motor.getCurrentDuty());

  // Test 8: setDeadzone()
  Serial.println("\n[8] Teste setDeadzone()");
  motor.setDeadzone(15);
  Serial.println("  - Deadzone auf 15 gesetzt");
  motor.changeSpeedAbsolute(10); // Sollte ignoriert werden
  delay(500);
  Serial.print("  - Duty nach changeSpeed(10): ");
  Serial.println(motor.getCurrentDuty());
  motor.setDeadzone(5); // Zurücksetzen

  // Test 9: setThreshold()
  Serial.println("\n[9] Teste setThreshold()");
  motor.setThreshold(30);
  Serial.println("  - Threshold auf 30 gesetzt");

  // Test 10: setThresholdTime()
  Serial.println("\n[10] Teste setThresholdTime()");
  motor.setThresholdTime(2000);
  Serial.println("  - Threshold Time auf 2000ms gesetzt");

  Serial.println("\n[Motor Tests abgeschlossen]");
  delay(2000);
}

void testSteeringServoFunctions() {
  printTestHeader("SteeringServo-Klasse");

  // Test 1: getCurrentSteeringDegree() - Initial
  Serial.println("\n[1] Teste getCurrentSteeringDegree() - Initial");
  Serial.print("  - Aktueller Winkel: ");
  Serial.println(steering.getCurrentSteeringDegree());

  // Test 2: setRestPosition()
  Serial.println("\n[2] Teste setRestPosition()");
  steering.setRestPosition();
  delay(1000);
  Serial.print("  - Winkel nach Reset: ");
  Serial.println(steering.getCurrentSteeringDegree());

  // Test 3: steer() - Rechts 25%
  Serial.println("\n[3] Teste steer() - 25% rechts");
  steering.steerAbsolute(25);
  delay(1000);
  Serial.print("  - Aktueller Winkel: ");
  Serial.println(steering.getCurrentSteeringDegree());

  // Test 4: steer() - Rechts 50%
  Serial.println("\n[4] Teste steer() - 50% rechts");
  steering.steerAbsolute(75);
  delay(1000);
  Serial.print("  - Aktueller Winkel: ");
  Serial.println(steering.getCurrentSteeringDegree());

  // Test 5: steer() - Rechts 100% (Maximum)
  Serial.println("\n[5] Teste steer() - 100% rechts (Maximum)");
  steering.steerAbsolute(100);
  delay(1000);
  Serial.print("  - Aktueller Winkel: ");
  Serial.println(steering.getCurrentSteeringDegree());

  // Test 6: steer() - Zurück zu 0
  Serial.println("\n[6] Teste steer() - Zurueck zu 0");
  steering.steerAbsolute(0);
  delay(1000);
  Serial.print("  - Aktueller Winkel: ");
  Serial.println(steering.getCurrentSteeringDegree());

  // Test 7: steer() - Links 25%
  Serial.println("\n[7] Teste steer() - 25% links");
  steering.steerAbsolute(-25);
  delay(1000);
  Serial.print("  - Aktueller Winkel: ");
  Serial.println(steering.getCurrentSteeringDegree());

  // Test 8: steer() - Links 50%
  Serial.println("\n[8] Teste steer() - 50% links");
  steering.steerAbsolute(-75);
  delay(1000);
  Serial.print("  - Aktueller Winkel: ");
  Serial.println(steering.getCurrentSteeringDegree());

  // Test 9: steer() - Links 100% (Maximum)
  Serial.println("\n[9] Teste steer() - 100% links (Maximum)");
  steering.steerAbsolute(-130);
  delay(1000);
  Serial.print("  - Aktueller Winkel: ");
  Serial.println(steering.getCurrentSteeringDegree());

  // Test 10: setRestPosition() - Finale Position
  Serial.println("\n[10] Teste setRestPosition() - Finale Position");
  steering.setRestPosition();
  delay(1000);
  Serial.print("  - Finaler Winkel: ");
  Serial.println(steering.getCurrentSteeringDegree());

  Serial.println("\n[SteeringServo Tests abgeschlossen]");
  delay(2000);
}

void testLEDManagerFunctions() {
  printTestHeader("LEDManager-Klasse");

  // Test 1: rest() - Alle LEDs in Ruheposition
  Serial.println("\n[1] Teste rest() - Alle LEDs");
  leftIndicator.rest();
  rightIndicator.rest();
  frontLights.rest();
  rearLights.rest();
  delay(2000);
  Serial.println("  - Alle LEDs in Ruheposition");

  // Test 2: turnOn() - Front Lights
  Serial.println("\n[2] Teste turnOn() - Frontlichter 100%");
  frontLights.turnOn(100);
  delay(2000);
  Serial.println("  - Frontlichter an (100%)");

  // Test 3: turnOn() - Rear Lights mit reduzierter Helligkeit
  Serial.println("\n[3] Teste turnOn() - Ruecklichter 50%");
  rearLights.turnOn(50);
  delay(2000);
  Serial.println("  - Ruecklichter an (50%)");

  // Test 4: turnOff() - Front Lights
  Serial.println("\n[4] Teste turnOff() - Frontlichter");
  frontLights.turnOff();
  delay(2000);
  Serial.println("  - Frontlichter aus");

  // Test 5: turnOff() - Rear Lights
  Serial.println("\n[5] Teste turnOff() - Ruecklichter");
  rearLights.turnOff();
  delay(2000);
  Serial.println("  - Ruecklichter aus");

  // Test 6: startIndicating() - Linker Blinker
  Serial.println("\n[6] Teste startIndicating() - Linker Blinker");
  leftIndicator.startIndicating();
  delay(5000);
  Serial.println("  - Linker Blinker blinkt...");

  // Test 7: stopIndicating() - Linker Blinker
  Serial.println("\n[7] Teste stopIndicating() - Linker Blinker");
  leftIndicator.stopIndicating();
  delay(2000);
  Serial.println("  - Linker Blinker gestoppt");

  // Test 8: startIndicating() - Rechter Blinker
  Serial.println("\n[8] Teste startIndicating() - Rechter Blinker");
  rightIndicator.startIndicating();
  delay(5000);
  Serial.println("  - Rechter Blinker blinkt...");

  // Test 9: stopIndicating() - Rechter Blinker
  Serial.println("\n[9] Teste stopIndicating() - Rechter Blinker");
  rightIndicator.stopIndicating();
  delay(2000);
  Serial.println("  - Rechter Blinker gestoppt");

  // Test 10: setIndicatorTiming() - Schnelleres Blinken
  Serial.println("\n[10] Teste setIndicatorTiming() - Schnelles Blinken (300ms)");
  leftIndicator.setIndicatorTiming(300);
  leftIndicator.startIndicating();
  delay(3000);
  leftIndicator.stopIndicating();
  Serial.println("  - Blinker mit 300ms Timing getestet");

  // Test 11: setIndicatorTiming() - Langsames Blinken
  Serial.println("\n[11] Teste setIndicatorTiming() - Langsames Blinken (1000ms)");
  leftIndicator.setIndicatorTiming(1000);
  leftIndicator.startIndicating();
  delay(4000);
  leftIndicator.stopIndicating();
  Serial.println("  - Blinker mit 1000ms Timing getestet");

  // Test 12: Beide Blinker gleichzeitig
  Serial.println("\n[12] Teste beide Blinker gleichzeitig (Warnblinker)");
  leftIndicator.setIndicatorTiming(500);
  rightIndicator.setIndicatorTiming(500);
  leftIndicator.startIndicating();
  rightIndicator.startIndicating();
  delay(5000);
  leftIndicator.stopIndicating();
  rightIndicator.stopIndicating();
  Serial.println("  - Warnblinker gestoppt");

  // Test 13: Alle LEDs gleichzeitig an
  Serial.println("\n[13] Teste alle LEDs gleichzeitig an");
  frontLights.turnOn(100);
  rearLights.turnOn(100);
  leftIndicator.turnOn(100);
  rightIndicator.turnOn(100);
  delay(3000);
  Serial.println("  - Alle LEDs an");

  // Test 14: Alle LEDs aus
  Serial.println("\n[14] Teste alle LEDs aus");
  frontLights.turnOff();
  rearLights.turnOff();
  leftIndicator.turnOff();
  rightIndicator.turnOff();
  delay(2000);
  Serial.println("  - Alle LEDs aus");

  // Test 15: Zurück zum Ruhezustand
  Serial.println("\n[15] Teste rest() - Zurueck zum Ruhezustand");
  frontLights.rest();
  rearLights.rest();
  leftIndicator.rest();
  rightIndicator.rest();
  delay(2000);
  Serial.println("  - Alle LEDs im Ruhezustand");

  Serial.println("\n[LEDManager Tests abgeschlossen]");
  delay(2000);
}

void setup() {
  Serial.begin(115200);
  delay(2000); // Zeit für Serial Monitor

  Serial.println("\n\n");
  Serial.println("########################################");
  Serial.println("#  BUGGYCONTROL UMFASSENDE TESTS      #");
  Serial.println("########################################");
  Serial.println();
  Serial.println("Initialisiere Hardware...");

  // Pins als Output setzen
  pinMode(16, OUTPUT); // Blinker links
  pinMode(19, OUTPUT); // Frontlicht
  pinMode(5, OUTPUT);  // Blinker rechts
  pinMode(18, OUTPUT); // Rücklicht
  pinMode(23, OUTPUT); // Servo
  pinMode(14, OUTPUT); // Motor Enable Front
  pinMode(27, OUTPUT); // Motor Enable Back

  Serial.println("Hardware initialisiert!");

  // WICHTIG: Servo initialisieren (muss in setup() aufgerufen werden!)
  Serial.println("Initialisiere Servo...");
  steering.begin();

  delay(2000);

  // Teste alle Klassen
  testMotorFunctions();
  testSteeringServoFunctions();
  testLEDManagerFunctions();

  // Abschließende Meldung
  Serial.println("\n\n");
  Serial.println("########################################");
  Serial.println("#  ALLE TESTS ABGESCHLOSSEN           #");
  Serial.println("########################################");
  Serial.println();
  Serial.println("Alle Funktionen wurden getestet!");
  Serial.println("Pruefe die Ausgabe fuer Details.");
}

void loop() {
  // Nichts zu tun
  delay(100);
}
