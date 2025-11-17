#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
private:
  int pwm_pin_front;  // Pin für Vorwärtsbewegung
  int pwm_pin_back;   // Pin für Rückwärtsbewegung

  int high_pin_front;
  int high_pin_back; 

  int max_duty;   // maximal erlaubter duty-Wert, <=100
  int min_duty;   // minimaler Duty-Wert, bei dem sich der Motor noch dreht
  int direction_change_delay;  // Sicherheitsdelay, wenn der Motor stoppt oder die Richtung wechselt
  int deadzone;   // Werte innerhalb von +-deadzone werden als 0 gewertet
  int threshold;  // Bei Änderungen von Duty > threshold wird Duty linear gefadet
  int threshold_time;  // Zeit, die der Threshold beansprucht. Nicht blockierend

  int freq;
  int current_duty;
  int last_duty;

  int checkDutyRange(int target_duty);
  void fadeDuty(int target_duty);
  void setDuty(int target_duty);

public:
  Motor(int pwm_pin_front, int pwm_pin_back, int high_pin_front, int high_pin_back, int max_duty, int min_duty,
        int direction_change_delay, int freq);
  Motor();  // Default-Konstruktor

  int getPin(int type);
  int getCurrentDuty();
  void changeSpeed(int direction_vector);
  void changeSpeedAbsolute(int target_duty);

  void setDeadzone(int dz);
  void setThreshold(int th);
  void setThresholdTime(int tt);
};

#endif // MOTOR_H
