#include "Motor.h"

Motor::Motor() : delayChecker() {
  // Default-Konstruktor für globale Initialisierung
  pin_front = 0;
  pin_back = 0;
  max_duty = 0;
  min_duty = 0;
  direction_change_delay = 0;
  freq = 0;
  deadzone = 5;
  threshold = 30;
  threshold_time = 300;
  current_duty = 0;
  time_passed = 0;
  start_time = 0;
}

Motor::Motor(int _pin_front, int _pin_back, int _max_duty, int _min_duty,
             int _direction_change_delay, int _freq) : delayChecker() {
  pin_front = _pin_front;
  pin_back = _pin_back;
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
  ledcAttach(pin_front, freq, 8);
  ledcAttach(pin_back, freq, 8);

  // Zur Sicherheit Pins 0 setzen
  ledcWrite(pin_front, 0);
  ledcWrite(pin_back, 0);
}

int Motor::getPin(int type) {
  if (type == -1) {
    return pin_back;
  } else if (type == 1) {
    return pin_front;
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
  Serial.print("checkDutyRange: target_duty input = ");
  Serial.println(target_duty);

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
  Serial.print("fadeDuty target_duty start = ");
  Serial.println(target_duty);

  target_duty = checkDutyRange(target_duty);

  if (target_duty > 0) {
    ledcWrite(pin_back, 0);
    ledcFade(pin_front, abs(current_duty) * 255 / 100, abs(target_duty) * 255 / 100, threshold_time);
  } else if (target_duty < 0) {
    ledcWrite(pin_front, 0);
    ledcFade(pin_back, abs(current_duty) * 255 / 100, abs(target_duty) * 255 / 100, threshold_time);
  } else {
    ledcWrite(pin_front, 0);
    ledcWrite(pin_back, 0);
    startDelay();
  }

  current_duty = target_duty;
  Serial.print("current_duty = ");
  Serial.println(current_duty);
}

bool Motor::checkDelay() {
  bool is_blocking;
  time_passed = millis() - start_time;
  if(time_passed <= direction_change_delay) {
    is_blocking = true;
  }
  else {
    is_blocking = false;
    delayChecker.detach();
  }
  return is_blocking;
}

static void checkDelayCallback(Motor* instance) {
  instance->checkDelay();
}

void Motor::startDelay() {
  start_time = millis();
  time_passed = 0;
  delayChecker.attach_ms(1, checkDelayCallback, this);
}

void Motor::setDuty(int target_duty) {
  target_duty = checkDutyRange(target_duty);
  Serial.print("aktualisierte Duty: ");
  Serial.println(target_duty);

  if (target_duty > 0) {
    ledcWrite(pin_back, 0);
    ledcWrite(pin_front, abs(target_duty) * 255 / 100);
    Serial.print("Duty auf ");
    Serial.print(target_duty);
    Serial.println(" bei pin_front");
  } else if (target_duty < 0) {
    ledcWrite(pin_front, 0);
    ledcWrite(pin_back, abs(target_duty) * 255 / 100);
    Serial.print("Duty auf ");
    Serial.print(target_duty);
    Serial.println(" bei pin_back");
  } else {
    startDelay();
    Serial.println("safety_delay initialisiert");
  }

  current_duty = target_duty;
}

void Motor::safetyDelay() {
  Serial.println("safety_delay gestartet");
  delay(direction_change_delay);
  ledcWrite(pin_front, 0);
  ledcWrite(pin_back, 0);
  Serial.println("safety_delay durchgeführt");
}

bool Motor::changeSpeed(int direction_vector) {
  Serial.print("changeSpeed mit direction_vector ");
  Serial.println(direction_vector);

  int target_duty = current_duty + direction_vector;

  last_duty = current_duty;

  if(checkDelay()) {
    return false;
  }

  // Prüfe auf Richtungswechsel und stoppe Motor erst
  if ((target_duty > 0 && current_duty < 0) || (target_duty < 0 && current_duty > 0)) {
    ledcWrite(pin_front, 0);
    ledcWrite(pin_back, 0);
    current_duty = 0;
    startDelay();
    Serial.println("Richtungswechsel erkannt - Motor gestoppt, Delay gestartet");
    return false;
  }

  if (target_duty > 0) {
    if (abs(target_duty - current_duty) < threshold) {
      setDuty(target_duty);
    } else {
      fadeDuty(target_duty);
    }
  } else if (target_duty < 0) {
    if (abs(target_duty - current_duty) < threshold) {
      setDuty(target_duty);
    } else {
      fadeDuty(target_duty);
    }
  } else {
    setDuty(0);
  }
  return true;
}
