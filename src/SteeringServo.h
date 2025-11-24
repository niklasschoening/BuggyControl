#ifndef STEERING_SERVO_H
#define STEERING_SERVO_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <LEDManager.h>

class SteeringServo {
private:
  int pin;  // Attach-Pin des Servos
  int power_pin;  // Power-Pin (optional)
  int rest_position;  // Position des Servos, bei dem der Buggy geradeaus fährt
  int max_steering_degree;  // maximaler Lenkwert
  int current_steering_degree;
  int deadzone;
  bool initialized;  // Flag ob begin() aufgerufen wurde

  int current_steering_percent;

  Servo servo;

  int percentToDegree(int steering_percent);

public:
  SteeringServo(int control_pin, int power_pin, int rest_position, int max_steering_degree, int deadzone);
  SteeringServo();  // Default-Konstruktor

  void begin();  // Muss in setup() aufgerufen werden!
  void steer(int steering_vector);
  void steerAbsolute(int steering_percent);
  int getCurrentSteeringDegree();
  int getCurrentSteeringPercent();
  void setRestPosition();
};

#endif // STEERING_SERVO_H
