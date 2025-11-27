# 🚗 BuggyControl

[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange)](https://platformio.org/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-blue)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/License-Private-red)](LICENSE)

Eine vollständige ESP32-Arduino-Bibliothek zur Steuerung eines RC-Buggys mit Motor, Servo-Lenkung, LED-Management und erweiterten Features wie Launch Control und sanftem Fading.

---

## ✨ Features

### 🏎️ Motor-Klasse
- ✅ PWM-basierte Motorsteuerung (vorwärts/rückwärts)
- ✅ Einstellbare Deadzone und Min/Max Duty Cycle
- ✅ Safety-Delay bei Richtungswechseln (nicht-blockierend)
- ✅ **Fading-Toggle**: Sanfte Geschwindigkeitsübergänge aktivierbar via `initFading()` / `uninitFading()`
- ✅ **Launch Control**: Sigmoid-basierte Beschleunigungskurve für optimalen Start
- ✅ Konfigurierbare Fade-Zeit und Threshold

### 🎮 SteeringServo-Klasse
- ✅ Servo-basierte Lenkung mit präziser Winkelsteuerung
- ✅ Konfigurierbare Rest-Position und maximaler Lenkwinkel
- ✅ Deadzone für präzise Geradeausfahrt
- ✅ Prozent-zu-Grad Konvertierung
- ✅ Optionale Stromversorgungssteuerung

### 💡 LEDManager-Klasse
- ✅ Verwaltung mehrerer LEDs pro Manager-Instanz
- ✅ Blinker-Funktion mit konfigurierbarem Timing
- ✅ Verschiedene Rest-States (an/aus)
- ✅ PWM-basierte Helligkeitssteuerung (0-100%)
- ✅ Nicht-blockierende Ticker-basierte Animationen

---

## 📦 Installation

### Für PlatformIO-Projekte

1. Klone das Repository oder kopiere den `src/` Ordner in dein Projekt
2. Füge die ESP32Servo Abhängigkeit in `platformio.ini` hinzu:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps =
    madhephaestus/ESP32Servo@^3.0.5
```

3. Kompiliere mit:
```bash
pio run -e esp32dev
```

### Als Arduino-Bibliothek (Typisch)

1. Lade das Repository als .Zip herunter
2. Füge es in Arduino IDE ein (Sketch -> include Library -> add .ZIP Library -> ZIP auswählen)

---

## 🚀 Quick Start

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
LEDManager leftBlinker({16}, 0, 100, 1000);
LEDManager rightBlinker({5}, 0, 100, 1000);

void setup() {
  Serial.begin(115200);

  // Servo muss in setup() initialisiert werden!
  steering.begin();

  // Optional: Fading aktivieren für sanfte Übergänge
  motor.initFading();
  motor.setFadeTime(500);  // 500ms Fade-Zeit

  // Teste Motor
  motor.changeSpeedAbsolute(50);  // 50% vorwärts
  delay(2000);
  motor.changeSpeedAbsolute(0);   // Stopp

  // Teste Lenkung
  steering.steerAbsolute(50);      // 50% rechts
  delay(1000);
  steering.setRestPosition();      // zurück zur Mitte

  // Teste Blinker
  leftBlinker.startIndicating();
  delay(3000);
  leftBlinker.stopIndicating();
}

void loop() {
  // Deine Loop-Logik hier
}
```

---

## 📚 API-Referenz

### Motor

#### Konstruktor
```cpp
Motor(int pwm_pin_front, int pwm_pin_back,
      int high_pin_front, int high_pin_back,
      int max_duty, int min_duty,
      int direction_change_delay, int freq)
```

#### Geschwindigkeitssteuerung
| Methode | Beschreibung |
|---------|--------------|
| `void changeSpeed(int direction_vector)` | Ändert Geschwindigkeit relativ um `direction_vector` (in %) |
| `void changeSpeedAbsolute(int target_duty)` | Setzt Geschwindigkeit absolut auf `target_duty` (-100 bis +100%) |
| `int getCurrentDuty()` | Gibt aktuellen Duty Cycle zurück (-100 bis +100%) |
| `void setZero()` | Stoppt den Motor sofort |

#### Fading-Funktionen
| Methode | Beschreibung |
|---------|--------------|
| `void initFading()` | Aktiviert sanfte Geschwindigkeitsübergänge |
| `void uninitFading()` | Deaktiviert Fading (direktes Springen) |
| `void setFadeTime(int ms)` | Setzt Fade-Dauer in Millisekunden |
| `void setThreshold(int th)` | Setzt Threshold ab dem gefadet wird (Standard: 30%) |

#### Launch Control
| Methode | Beschreibung |
|---------|--------------|
| `bool startLaunchControl()` | Startet Launch Control (Sigmoid-Beschleunigung) |
| `bool stopLaunchControl()` | Stoppt Launch Control |
| `void setLcTime(int ms)` | Setzt Launch Control Dauer |
| `void setAggression(int a)` | Setzt Aggressivität der Beschleunigungskurve |

#### Konfiguration
| Methode | Beschreibung |
|---------|--------------|
| `void setDeadzone(int dz)` | Setzt Deadzone (Standard: 5%) |
| `int getPin(int type)` | Gibt PWM/High-Pin zurück (type: 1=front, -1=back, 2=high_front, -2=high_back) |

---

### SteeringServo

#### Konstruktor
```cpp
SteeringServo(int control_pin, int power_pin,
              int rest_position, int max_steering_degree,
              int deadzone)
```
- `power_pin`: Pin für Servo-Stromversorgung (-1 wenn nicht verwendet)

#### Methoden
| Methode | Beschreibung |
|---------|--------------|
| `void begin()` | **WICHTIG:** Muss in `setup()` aufgerufen werden! |
| `void steer(int steering_vector)` | Lenkt relativ um `steering_vector` (in %) |
| `void steerAbsolute(int steering_percent)` | Lenkt absolut auf Position (-100 bis +100%) |
| `void setRestPosition()` | Setzt Servo zurück zur Rest-Position |
| `int getCurrentSteeringDegree()` | Gibt aktuelle Position in Grad zurück |

---

### LEDManager

#### Konstruktor
```cpp
LEDManager(std::vector<int> leds, int rest_state,
           int brightness, int freq)
```
- `leds`: Vector mit LED-Pin-Nummern
- `rest_state`: 0 = aus, 1 = an
- `brightness`: 0-100%
- `freq`: PWM-Frequenz in Hz

#### Methoden
| Methode | Beschreibung |
|---------|--------------|
| `void turnOn(int brightness)` | Schaltet LEDs mit Helligkeit ein (0-100%) |
| `void turnOff()` | Schaltet LEDs aus |
| `void startIndicating()` | Startet Blinker-Animation (nicht-blockierend) |
| `void stopIndicating()` | Stoppt Blinker-Animation |
| `void rest()` | Setzt LEDs in konfigurierten Rest-State |
| `void setIndicatorTiming(int timing_ms)` | Setzt Blinker-Intervall in ms |

---

## 🧪 Tests & Beispiele

### Comprehensive Test
Ein vollständiger Hardware-Test mit 11 Szenarien:
```bash
pio run -t upload -e esp32dev
```

Das Testprogramm befindet sich in `examples/ComprehensiveTest/ComprehensiveTest.ino` und testet:

1. ✅ Geradeausfahrt mit verschiedenen Geschwindigkeiten
2. ✅ Linkskurve mit Blinker
3. ✅ Rechtskurve mit Blinker
4. ✅ Slalom-Fahrt
5. ✅ Rückwärts Einparken
6. ✅ Notbremsung
7. ✅ Nachtfahrt mit Lichtsteuerung
8. ✅ Geschwindigkeitstest
9. ✅ Lenkungstest
10. ✅ LED-Funktionstest
11. ✅ **Fading-Test** (MIT vs. OHNE Fading)

### Unit Tests
```bash
pio test -e esp32dev
```

---

## 🔌 Hardware-Setup

### Benötigte Komponenten
- ESP32 Development Board
- DC-Motor mit H-Bridge (z.B. L298N, L293D)
- Servo für Lenkung (Standard 50Hz PWM)
- LEDs für Beleuchtung/Blinker
- Widerstände (220Ω für LEDs empfohlen)
- Externe Spannungsversorgung für Motor (z.B. 7.4V LiPo)

### Beispiel Pin-Belegung

| Komponente | Pin | Beschreibung |
|------------|-----|--------------|
| Motor PWM Vorwärts | 13 | H-Bridge IN1 |
| Motor PWM Rückwärts | 12 | H-Bridge IN2 |
| Motor Enable Vorwärts | 14 | H-Bridge EN1 |
| Motor Enable Rückwärts | 27 | H-Bridge EN2 |
| Servo | 23 | Lenkservo Signal |
| Blinker Links | 16 | LED (mit Widerstand) |
| Blinker Rechts | 5 | LED (mit Widerstand) |
| Frontlicht | 19 | LED (mit Widerstand) |
| Rücklicht | 18 | LED (mit Widerstand) |

### Schaltplan

```
ESP32          H-Bridge (L298N)          Motor
-----          ----------------          -----
Pin 13  ----->  IN1                        M+
Pin 12  ----->  IN2                        M-
Pin 14  ----->  EN1
Pin 27  ----->  EN2
GND     ----->  GND
            |
            +-> 7.4V Battery
```

---

## ⚙️ LEDC-Kanal-Verwaltung

Die Bibliothek nutzt ESP32 LEDC-Kanäle intelligent:

| Komponente | Kanäle | Timer |
|------------|--------|-------|
| Motor | 2-3 | Timer 1 |
| Servo | 0-15 (ESP32Servo) | Timer 0 |
| LEDs | 4+ | Timer 2+ |

**Wichtig:** Motor nutzt Timer 1 um Konflikte mit Servo (Timer 0, 50Hz) zu vermeiden!

---

## 🎯 Neue Features in 1.0.1

### Fading-Toggle
```cpp
motor.initFading();        // Aktiviert sanfte Übergänge
motor.setFadeTime(500);    // 500ms Fade-Zeit
motor.changeSpeedAbsolute(80);  // Faded sanft auf 80%

motor.uninitFading();      // Deaktiviert Fading
motor.changeSpeedAbsolute(80);  // Springt direkt auf 80%
```

### Launch Control
```cpp
motor.startLaunchControl();  // Startet optimierte Beschleunigung
// Motor beschleunigt automatisch mit Sigmoid-Kurve
delay(1000);
motor.stopLaunchControl();   // Stoppt Launch Control
```

## 📝 Changelog

### [1.0.1-pre] - 2025-11-27
- ✅ Fading-Toggle Funktionalität hinzugefügt (`initFading()` / `uninitFading()`)
- ✅ Korrektur der Fade-Ramp-Berechnung
- ✅ Fix für fehlenden Return-Wert in `Motor::launchControl()`
- ✅ Comprehensive Test um Fading-Szenario erweitert
- ✅ Test-Fixes für Motor-Konstruktor

### [1.0.0-pre] - 2025-11-27
- 🎉 Initial Pre-Release

---

## 📄 Lizenz

Privates Projekt für eigene Verwendung.

---

## 👤 Autor

**Niklas Schöning**

---

## 🙏 Credits

- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) - Servo-Steuerung für ESP32
- [PlatformIO](https://platformio.org/) - Build-System
- [Arduino Framework](https://www.arduino.cc/) - Basis-Framework
