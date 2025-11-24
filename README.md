# BuggyControl

Eine ESP32 Arduino-Bibliothek zur Steuerung eines RC-Buggys mit Motor, Servo-Lenkung und LED-Management.

## Features

### Motor-Klasse
- PWM-basierte Motorsteuerung (vorwärts/rückwärts)
- Einstellbare Deadzone und Min/Max Duty Cycle
- Safety-Delay bei Richtungswechseln
- Automatisches Fading bei großen Geschwindigkeitsänderungen

### SteeringServo-Klasse
- Servo-basierte Lenkung
- Konfigurierbare Rest-Position und maximaler Lenkwinkel
- Deadzone für präzise Geradeausfahrt
- Prozent-zu-Grad Konvertierung

### LEDManager-Klasse
- Verwaltung mehrerer LEDs pro Manager-Instanz
- Blinker-Funktion mit konfigurierbarem Timing
- Verschiedene Rest-States (an/aus)
- Einstellbare Helligkeit

## Installation

### Für PlatformIO-Projekte

1. Kopiere den `lib/BuggyControl` Ordner in dein Projekt's `lib/` Verzeichnis
2. Füge die ESP32Servo Abhängigkeit in `platformio.ini` hinzu:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps =
    madhephaestus/ESP32Servo@^3.0.5
```

### Für lokale Nutzung

Dieses Projekt ist bereits als PlatformIO-Projekt konfiguriert. Du kannst es direkt kompilieren mit:

```bash
platformio run
```

## Verwendung

```cpp
#include <Arduino.h>
#include <BuggyControl.h>

// Motor initialisieren
Motor motor(13,      // pwm_pin_front
            12,      // pwm_pin_back
            14,      // high_pin_front (Enable Pin)
            27,      // high_pin_back (Enable Pin)
            100,     // max_duty (%)
            30,      // min_duty (%)
            1000,    // direction_change_delay (ms)
            10000);  // freq (Hz)

// Servo initialisieren
SteeringServo steering(23,   // control_pin
                       -1,   // power_pin (-1 wenn nicht verwendet)
                       90,   // rest_position (Grad)
                       20,   // max_steering_degree
                       6);   // deadzone

// LEDs initialisieren
LEDManager leftIndicator({16}, 0, 100, 1000);    // Pin 16, aus, 100% brightness, 1kHz
LEDManager rightIndicator({5}, 0, 100, 1000);    // Pin 5

void setup() {
  Serial.begin(115200);

  // Teste Motor
  motor.changeSpeed(50);  // 50% vorwärts
  delay(2000);
  motor.changeSpeed(-50); // Stopp

  // Teste Lenkung
  steering.steer(50);  // 50% rechts
  delay(1000);
  steering.setRestPosition(); // zurück zur Mitte

  // Teste Blinker
  leftIndicator.startIndicating();
  delay(3000);
  leftIndicator.stopIndicating();
}

void loop() {
  // Deine Loop-Logik
}
```

## Klassen-Referenz

### Motor

**Konstruktor:**
```cpp
Motor(int pwm_pin_front, int pwm_pin_back, int high_pin_front, int high_pin_back,
      int max_duty, int min_duty, int direction_change_delay, int freq)
```

**Methoden:**
- `void changeSpeed(int direction_vector)` - Ändert die Geschwindigkeit relativ um direction_vector (in %)
- `void changeSpeedAbsolute(int target_duty)` - Setzt die Geschwindigkeit absolut auf target_duty (in %)
- `int getCurrentDuty()` - Gibt aktuellen Duty Cycle zurück (-100 bis +100)
- `int getPin(int type)` - Gibt PWM-Pin zurück (type: 1=front, -1=back)
- `void setDeadzone(int dz)` - Setzt die Deadzone
- `void setThreshold(int th)` - Setzt den Threshold für Fading
- `void setThresholdTime(int tt)` - Setzt die Fading-Zeit in ms

### SteeringServo

**Konstruktor:**
```cpp
SteeringServo(int control_pin, int power_pin, int rest_position,
              int max_steering_degree, int deadzone)
```
- `power_pin`: Pin zum Schalten der Servo-Stromversorgung (-1 wenn nicht verwendet)

**Methoden:**
- `void steer(int steering_vector)` - Lenkt relativ um steering_vector (in %)
- `void steerAbsolute(int steering_percent)` - Lenkt absolut auf Position (-100 bis +100%)
- `void setRestPosition()` - Setzt Servo zurück zur Rest-Position
- `int getCurrentSteeringDegree()` - Gibt aktuelle Position in Grad zurück

### LEDManager

**Konstruktor:**
```cpp
LEDManager(std::vector<int> leds, int rest_state, int brightness, int freq)
```
- `rest_state`: 0 = aus, 1 = an
- `brightness`: 0-100%

**Methoden:**
- `void turnOn(int brightness)` - Schaltet LEDs auf Helligkeit ein
- `void turnOff()` - Schaltet LEDs aus
- `void startIndicating()` - Startet Blinker-Animation
- `void stopIndicating()` - Stoppt Blinker-Animation
- `void rest()` - Setzt LEDs in Rest-State
- `void setIndicatorTiming(int timing_ms)` - Setzt Blinker-Timing

## Hardware-Anforderungen

- ESP32 Development Board
- DC-Motor mit H-Bridge (z.B. L298N, L293D)
- Servo für Lenkung
- LEDs für Beleuchtung/Blinker
- Entsprechende Widerstände und Spannungsversorgung

## Pin-Belegung im Beispiel

- Motor: Pin 13 (vorwärts), Pin 12 (rückwärts)
- Servo: Pin 23
- Linker Blinker: Pin 16
- Rechter Blinker: Pin 5
- Frontlicht: Pin 19
- Rücklicht: Pin 18

## LEDC-Kanal-Verwaltung

Die Bibliothek verwendet automatisch LEDC-Kanäle:
- Motor: Kanäle 0-1
- LEDs: Kanäle 2+

## Lizenz

Privates Projekt für eigene Verwendung.

## Autor

Niklas Schöning
