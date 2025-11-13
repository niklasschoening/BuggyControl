#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Ticker.h>

class Motor {
private:
  int pin_front;  // Pin für Vorwärtsbewegung
  int pin_back;   // Pin für Rückwärtsbewegung

  int max_duty;   // maximal erlaubter duty-Wert, <=100
  int min_duty;   // minimaler Duty-Wert, bei dem sich der Motor noch dreht
  int direction_change_delay;  // Sicherheitsdelay, wenn der Motor stoppt oder die Richtung wechselt
  int deadzone;   // Werte innerhalb von +-deadzone werden als 0 gewertet
  int threshold;  // Bei Änderungen von Duty > threshold wird Duty linear gefadet
  int threshold_time;  // Zeit, die der Threshold beansprucht. Nicht blockierend

  int freq;
  int current_duty;
  int last_duty;

  int time_passed;
  int start_time;

  Ticker delayChecker;

  int checkDutyRange(int target_duty);
  void fadeDuty(int target_duty);
  void setDuty(int target_duty);
  void safetyDelay();
  void startDelay();

public:
  Motor(int pin_front, int pin_back, int max_duty, int min_duty,
        int direction_change_delay, int freq);
  Motor();  // Default-Konstruktor

  int getPin(int type);
  int getCurrentDuty();
  bool changeSpeed(int direction_vector);

  void setDeadzone(int dz);
  void setThreshold(int th);
  void setThresholdTime(int tt);
  bool checkDelay();
};

#endif // MOTOR_H
