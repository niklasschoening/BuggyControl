#include "SteeringServo.h"

SteeringServo::SteeringServo() {
  // Default-Konstruktor für globale Initialisierung
  pin = 0;
  power_pin = -1;
  rest_position = 90;
  current_steering_degree = 90;
  max_steering_degree = 0;
  current_steering_percent = 0;
  deadzone = 0;
  initialized = false;
}

SteeringServo::SteeringServo(int control_pin,
                             int _power_pin,
                             int _rest_position,
                             int _max_steering_degree,
                             int _deadzone) {
  pin = control_pin;
  power_pin = _power_pin;
  rest_position = _rest_position;
  current_steering_degree = rest_position;
  max_steering_degree = _max_steering_degree;
  deadzone = _deadzone;
  initialized = false;
  // NICHT hier servo.attach() aufrufen! Das muss in begin() passieren.
}

void SteeringServo::begin() {
  // Power-Pin aktivieren falls vorhanden
  if(power_pin >= 0) {
    pinMode(power_pin, OUTPUT);
    digitalWrite(power_pin, HIGH);
  }

  // Servo initialisieren
  servo.attach(pin);
  delay(50);
  servo.write(rest_position);
  current_steering_degree = rest_position;
  initialized = true;
}

int SteeringServo::percentToDegree(int steering_percent) {
  int steering_degree = (steering_percent * max_steering_degree) / 100;
  return steering_degree;
}

int SteeringServo::getCurrentSteeringDegree() {
  return current_steering_degree;
}

int SteeringServo::getCurrentSteeringPercent()
{
  return current_steering_percent;
}

void SteeringServo::steerAbsolute(int steering_percent) {
  // Begrenze Eingabe auf ±100%
  if (steering_percent > 100) {
    steering_percent = 100;
  } else if (steering_percent < -100) {
    steering_percent = -100;
  }

  // Konvertiere Prozent zu absolutem Winkel
  int steering_degree = percentToDegree(steering_percent);
  int target_steering_degree = rest_position + steering_degree;
  
  if(abs(current_steering_degree - target_steering_degree) < 5 && target_steering_degree != rest_position + max_steering_degree && target_steering_degree != rest_position - max_steering_degree)
  {
    return;
  }
  current_steering_percent = steering_percent;

  // Deadzone-Prüfung

  if (abs(rest_position - target_steering_degree) <= deadzone) {
    target_steering_degree = rest_position;
    current_steering_percent = 0;
  }

  servo.write(target_steering_degree);
  current_steering_degree = target_steering_degree;
}

void SteeringServo::setRestPosition() {
  servo.write(rest_position);
  current_steering_degree = rest_position;
}
