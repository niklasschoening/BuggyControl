#include "Motor.h"

Motor::Motor() {
  // Default-Konstruktor für globale Initialisierung
  pwm_pin_front = 0;
  pwm_pin_back = 0;
  high_pin_front = 0;
  high_pin_back = 0;
  max_duty = 0;
  min_duty = 0;
  direction_change_delay = 0;
  freq = 0;
  deadzone = 5;
  threshold = 40;
  threshold_time = 300;
  current_duty = 0;
}

Motor::Motor(int _pwm_pin_front, int _pwm_pin_back, int _high_pin_front, int _high_pin_back, int _max_duty, int _min_duty,
             int _direction_change_delay, int _freq) {
  pwm_pin_front = _pwm_pin_front;
  pwm_pin_back = _pwm_pin_back;
  high_pin_back = _high_pin_back;
  high_pin_front = _high_pin_front;
  max_duty = _max_duty;
  min_duty = _min_duty;
  freq = _freq;
  direction_change_delay = _direction_change_delay;
  deadzone = 5;
  threshold = 30;
  threshold_time = 300;
  current_duty = 0;

  last_duty = 0;

  // Konfiguration der jeweiligen Pins
  ledcAttach(pwm_pin_front, freq, 8);
  ledcAttach(pwm_pin_back, freq, 8);

  pinMode(high_pin_front, OUTPUT);
  pinMode(high_pin_back, OUTPUT);

  // Zur Sicherheit Pins 0 setzen
  ledcWrite(pwm_pin_front, 0);
  ledcWrite(pwm_pin_back, 0);

  digitalWrite(high_pin_front, LOW);
  digitalWrite(high_pin_back, LOW);
}

int Motor::getPin(int type) {
  if (type == -1) {
    return pwm_pin_back;
  } else if (type == 1) {
    return pwm_pin_front;
  } else {
    return 0;
  }
}

int Motor::getCurrentDuty() {
  return current_duty;
}

void Motor::setDeadzone(int dz) {
  deadzone = dz;
}

void Motor::setThreshold(int th) {
  threshold = th;
}

void Motor::setThresholdTime(int tt) {
  threshold_time = tt;
}

int Motor::checkDutyRange(int target_duty) {
  if (abs(target_duty) < deadzone) {
    return 0;
  } else if (abs(target_duty) < min_duty) {
    if (target_duty > 0) {
      return min_duty;
    } else {
      return -min_duty;
    }
  } else if (abs(target_duty) > max_duty) {
    if (target_duty > 0) {
      return max_duty;
    } else {
      return -max_duty;
    }
  } else {
    return target_duty;
  }
}

void Motor::fadeDuty(int target_duty) {
  target_duty = checkDutyRange(target_duty);

  // Prüfe auf Richtungswechsel über die Null
  if ((target_duty > 0 && current_duty < 0) || (target_duty < 0 && current_duty > 0)) {
    ledcWrite(pwm_pin_front, 0);
    digitalWrite(high_pin_front, LOW);
    ledcWrite(pwm_pin_back, 0);
    digitalWrite(high_pin_back, LOW);
    current_duty = 0;
    delay(direction_change_delay);
  }

  if (target_duty > 0) {
    ledcWrite(pwm_pin_back, 0);
    digitalWrite(high_pin_back, LOW);

    digitalWrite(high_pin_front, HIGH);
    ledcFade(pwm_pin_front, abs(current_duty) * 255 / 100, abs(target_duty) * 255 / 100, threshold_time);
    delay(threshold_time);
    current_duty = target_duty;
  } else if (target_duty < 0) {
    ledcWrite(pwm_pin_front, 0);
    digitalWrite(high_pin_front, LOW);

    digitalWrite(high_pin_back, HIGH);
    ledcFade(pwm_pin_back, abs(current_duty) * 255 / 100, abs(target_duty) * 255 / 100, threshold_time);
    delay(threshold_time);
    current_duty = target_duty;
  } else {
    // target_duty == 0
    ledcWrite(pwm_pin_front, 0);
    digitalWrite(high_pin_front, LOW);
    ledcWrite(pwm_pin_back, 0);
    digitalWrite(high_pin_back, LOW);
    delay(direction_change_delay);
    current_duty = 0;
  }
}


void Motor::setDuty(int target_duty) {
  target_duty = checkDutyRange(target_duty);

  // Prüfe auf Richtungswechsel über die Null
  if ((target_duty > 0 && current_duty < 0) || (target_duty < 0 && current_duty > 0)) {
    ledcWrite(pwm_pin_front, 0);
    digitalWrite(high_pin_front, LOW);
    ledcWrite(pwm_pin_back, 0);
    digitalWrite(high_pin_back, LOW);
    current_duty = 0;
    delay(direction_change_delay);
  }

  if (target_duty > 0) {
    ledcWrite(pwm_pin_back, 0);
    digitalWrite(high_pin_back, LOW);

    ledcWrite(pwm_pin_front, abs(target_duty) * 255 / 100);
    digitalWrite(high_pin_front, HIGH);
    current_duty = target_duty;
  } else if (target_duty < 0) {
    ledcWrite(pwm_pin_front, 0);
    digitalWrite(high_pin_front, LOW);

    ledcWrite(pwm_pin_back, abs(target_duty) * 255 / 100);
    digitalWrite(high_pin_back, HIGH);
    current_duty = target_duty;
  } else {
    // target_duty == 0
    ledcWrite(pwm_pin_front, 0);
    digitalWrite(high_pin_front, LOW);
    ledcWrite(pwm_pin_back, 0);
    digitalWrite(high_pin_back, LOW);
    delay(direction_change_delay);
    current_duty = 0;
  }
}

void Motor::changeSpeed(int direction_vector) {
  int target_duty = current_duty + direction_vector;

  last_duty = current_duty;

  // Entscheide ob setDuty oder fadeDuty basierend auf threshold
  if (abs(target_duty - current_duty) < threshold) {
    setDuty(target_duty);
  } else {
    fadeDuty(target_duty);
  }
}

void Motor::changeSpeedAbsolute(int target_duty)
{
  target_duty = checkDutyRange(target_duty);

  if(abs(target_duty - current_duty) <= 3)
  {
    return;
  }

  // Entscheide ob setDuty oder fadeDuty basierend auf threshold
  // Richtungswechsel-Logik ist bereits in setDuty() und fadeDuty() eingebaut
  if(abs(current_duty - target_duty) >= threshold)
  {
    fadeDuty(target_duty);
  }
  else
  {
    setDuty(target_duty);
  }
}
