# BuggyControl Bibliothek - Installation und Verwendung

## Installation in der Arduino IDE

### Methode 1: Automatisch über Arduino Library Manager (wenn veröffentlicht)
1. Öffne die Arduino IDE
2. Gehe zu **Sketch → Bibliothek einbinden → Bibliotheken verwalten...**
3. Suche nach "BuggyControl"
4. Klicke auf "Installieren"

### Methode 2: Manuell (Aktuell)

Die Bibliothek ist bereits im korrekten Arduino-Bibliotheksordner installiert:
```
~/Documents/Arduino/libraries/BuggyControl/
```

#### Wenn du die Bibliothek neu installieren möchtest:

1. **Option A: Bereits vorhanden**
   - Die Bibliothek ist bereits unter `~/Documents/Arduino/libraries/BuggyControl/` installiert
   - Starte die Arduino IDE neu
   - Die Bibliothek sollte automatisch erkannt werden

2. **Option B: Von GitHub/Zip**
   - Lade die Bibliothek als ZIP herunter
   - Öffne die Arduino IDE
   - Gehe zu **Sketch → Bibliothek einbinden → .ZIP-Bibliothek hinzufügen...**
   - Wähle die heruntergeladene ZIP-Datei aus

3. **Option C: Manuelles Kopieren**
   ```bash
   # Kopiere die Bibliothek in den Arduino-Bibliotheksordner
   cp -r /pfad/zu/BuggyControl ~/Documents/Arduino/libraries/
   ```

## Bibliothek in der Arduino IDE verwenden

### 1. Bibliothek öffnen
1. Starte die Arduino IDE
2. Gehe zu **Datei → Beispiele → BuggyControl**
3. Wähle eines der Beispiele:
   - **BasicExample**: Einfaches Beispiel zum Testen aller Funktionen
   - **ComprehensiveTest**: Umfassendes Testprogramm mit 10 Szenarien

### 2. Board und Port einstellen
1. Gehe zu **Tools → Board → ESP32 Arduino**
2. Wähle dein ESP32-Board (z.B. "ESP32 Dev Module")
3. Gehe zu **Tools → Port**
4. Wähle den USB-Port deines ESP32 (z.B. `/dev/cu.usbserial-0001`)

### 3. Abhängigkeiten installieren
Die BuggyControl-Bibliothek benötigt die ESP32Servo-Bibliothek:

1. Gehe zu **Sketch → Bibliothek einbinden → Bibliotheken verwalten...**
2. Suche nach "ESP32Servo"
3. Installiere "ESP32Servo" von Jarrett (Version 3.0.5 oder höher)

### 4. Programm hochladen
1. Klicke auf den **Upload-Button** (Pfeil nach rechts) in der Arduino IDE
2. Warte bis der Upload abgeschlossen ist
3. Öffne den **Seriellen Monitor** (Tools → Serieller Monitor oder Strg+Shift+M)
4. Stelle die Baudrate auf **115200** ein
5. Beobachte die Testausgaben

## Verwendung in eigenen Projekten

### Grundlegendes Sketch-Template

```cpp
#include <Arduino.h>
#include <BuggyControl.h>

// Motor initialisieren
Motor motor(13, 12, 100, 30, 1000, 10000);

// Servo initialisieren
SteeringServo steering(23, 90, 20, 6);

// LEDs initialisieren
LEDManager leftIndicator({16}, 0, 100, 1000);
LEDManager rightIndicator({5}, 0, 100, 1000);
LEDManager frontLights({19}, 1, 100, 1000);
LEDManager rearLights({18}, 1, 100, 1000);

void setup() {
  Serial.begin(115200);

  // Pins als Output setzen
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(23, OUTPUT);

  // Deine Initialisierung hier...
}

void loop() {
  // Deine Steuerungslogik hier...
}
```

## Hardware-Verbindungen

### ESP32 Pinbelegung
| Komponente | Pin | Beschreibung |
|------------|-----|--------------|
| Motor Vorwärts | 13 | H-Bridge Input 1 |
| Motor Rückwärts | 12 | H-Bridge Input 2 |
| Servo | 23 | PWM für Lenkservo |
| Blinker Links | 16 | LED für linken Blinker |
| Blinker Rechts | 5 | LED für rechten Blinker |
| Frontlichter | 19 | LED für Scheinwerfer |
| Rücklichter | 18 | LED für Rücklichter |

## Beispiel-Szenarien

Das [ComprehensiveTest.ino](examples/ComprehensiveTest/ComprehensiveTest.ino) demonstriert:

1. **Geradeausfahrt** - Beschleunigen und Bremsen
2. **Linkskurve** - Mit Blinker
3. **Rechtskurve** - Mit Blinker
4. **Slalom-Fahrt** - Abwechselnd links/rechts
5. **Rückwärts Einparken** - Mit Warnblinker
6. **Notbremsung** - Mit Warnblinker
7. **Nachtfahrt** - Mit Lichtsteuerung
8. **Geschwindigkeitstest** - Alle Geschwindigkeitsstufen
9. **Lenkungstest** - Vollständiger Lenkbereich
10. **LED-Test** - Alle Lichtfunktionen

## Problembehandlung

### Bibliothek wird nicht gefunden
- Stelle sicher, dass die Bibliothek unter `~/Documents/Arduino/libraries/BuggyControl/` liegt
- Starte die Arduino IDE neu
- Überprüfe, dass die Ordnerstruktur korrekt ist:
  ```
  BuggyControl/
  ├── src/
  │   ├── BuggyControl.h
  │   ├── Motor.h
  │   ├── Motor.cpp
  │   ├── SteeringServo.h
  │   ├── SteeringServo.cpp
  │   ├── LEDManager.h
  │   └── LEDManager.cpp
  ├── examples/
  ├── library.properties
  └── library.json
  ```

### ESP32Servo nicht gefunden
- Installiere die ESP32Servo-Bibliothek über den Library Manager
- Version 3.0.5 oder höher wird empfohlen

### Upload-Fehler
- Überprüfe, ob der richtige Port ausgewählt ist
- Stelle sicher, dass kein anderes Programm den Port verwendet
- Drücke die BOOT-Taste am ESP32 beim Upload (falls nötig)

### Motor/Servo reagiert nicht
- Überprüfe die Verkabelung
- Stelle sicher, dass die GPIO-Pins korrekt konfiguriert sind
- Prüfe die Stromversorgung der externen Komponenten

## Weitere Ressourcen

- [API-Dokumentation](README.md)
- [Beispiele](examples/)
- GitHub Repository: [Link hier einfügen]

## Support

Bei Fragen oder Problemen:
- Öffne ein Issue auf GitHub
- Kontaktiere den Entwickler

## Lizenz

[Lizenzinformationen hier einfügen]
