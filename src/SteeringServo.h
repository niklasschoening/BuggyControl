#ifndef STEERING_SERVO_H
#define STEERING_SERVO_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <LEDManager.h>

class SteeringServo {
private:
  int pin;  // Attach-Pin des Servos
  int rest_position;  // Position des Servos, bei dem der Buggy geradeaus fährt
  int max_steering_degree;  // maximaler Lenkwert
  int current_steering_degree;
  int deadzone;

  Servo servo;

  int percentToDegree(int steering_percent);

public:
  SteeringServo(int pin, int rest_position, int max_steering_degree, int deadzone);
  SteeringServo();  // Default-Konstruktor

  void steer(int steering_vector);
  int getCurrentSteeringDegree();
  void setRestPosition();
};

#endif // STEERING_SERVO_H
