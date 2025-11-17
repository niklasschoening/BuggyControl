#include "SteeringServo.h"

SteeringServo::SteeringServo() {
  // Default-Konstruktor für globale Initialisierung
  pin = 0;
  rest_position = 90;
  current_steering_degree = 90;
  max_steering_degree = 0;
  deadzone = 0;
}

SteeringServo::SteeringServo(int control_pin,int power_pin, int _rest_position, int _max_steering_degree, int _deadzone) {
  pin = control_pin;
  if(power_pin >= 0) {
    pinMode(power_pin, OUTPUT);
    digitalWrite(power_pin, HIGH);
  }
  rest_position = _rest_position;
  current_steering_degree = rest_position;
  max_steering_degree = _max_steering_degree;
  deadzone = _deadzone;

  servo.attach(pin);
  delay(500);
  servo.write(rest_position);
}

void SteeringServo::steer(int steering_vector) {
  steering_vector = percentToDegree(steering_vector);
  int target_steering_degree = current_steering_degree + steering_vector;

  if (target_steering_degree < rest_position - max_steering_degree) {
    target_steering_degree = rest_position - max_steering_degree;
  } else if (target_steering_degree > rest_position + max_steering_degree) {
    target_steering_degree = rest_position + max_steering_degree;
  } else if (target_steering_degree < rest_position + deadzone && target_steering_degree > rest_position) {
    target_steering_degree = rest_position;
  } else if (target_steering_degree > rest_position - deadzone && target_steering_degree < rest_position) {
    target_steering_degree = rest_position;
  }

  servo.write(target_steering_degree);
  current_steering_degree = target_steering_degree;
}

int SteeringServo::percentToDegree(int steering_percent) {
  int steering_degree = (steering_percent * max_steering_degree) / 100;
  return steering_degree;
}

int SteeringServo::getCurrentSteeringDegree() {
  return current_steering_degree;
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

  // Deadzone-Prüfung
  if (abs(steering_percent) * max_steering_degree / 100 < deadzone) {
    target_steering_degree = rest_position;
  }

  servo.write(target_steering_degree);
  current_steering_degree = target_steering_degree;
}

void SteeringServo::setRestPosition() {
  servo.write(rest_position);
  current_steering_degree = rest_position;
}
