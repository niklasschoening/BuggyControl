#include "Motor.h"
#include <cmath>

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
  threshold_time = 3;
  current_duty = 0;
  last_duty = 0;
  lc_last_call = 0;
  lc_start_time = millis();
  is_launching = false;

  break_fade_threshhold = 5;
  fade_ticker_timing = 10;
  fade_time = 300;
  is_fading = false;
  fade_start_time = millis();
  fade_current_time = millis();
  fade_start_duty = 0;
  fade_target_duty = 0;

  is_blocking = false;
  delay_start_time = millis();

  should_fade = false;

  a = 1;
  T = 1000;
}

Motor::Motor(int _pwm_pin_front, 
             int _pwm_pin_back,
             int _high_pin_front, 
             int _high_pin_back, 
             int _max_duty, 
             int _min_duty,
             int _direction_change_delay, 
             int _freq
             ) 
{
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
  threshold_time = 3;
  current_duty = 0;

  last_duty = 0;

  // Launch Control Variablen initialisieren
  lc_last_call = 0;
  lc_current_time = 0;
  lc_start_time = 0;
  is_launching = false;
  a = 1;
  T = 1000;

  should_fade = false;

  // Konfiguration der jeweiligen Pins nur bei valider Initialisierung
  // Verwende Timer 1 (Kanäle 2-3) um Konflikt mit Servo (Timer 0, 50Hz) zu vermeiden
  if(freq > 0 && max_duty > 0) {
    ledcAttachChannel(pwm_pin_front, freq, 8, 2);
    ledcAttachChannel(pwm_pin_back, freq, 8, 3);

    pinMode(high_pin_front, OUTPUT);
    pinMode(high_pin_back, OUTPUT);

    // Zur Sicherheit Pins 0 setzen
    ledcWrite(pwm_pin_front, 0);
    ledcWrite(pwm_pin_back, 0);

    digitalWrite(high_pin_front, LOW);
    digitalWrite(high_pin_back, LOW);

    current_duty = 0;
    setDuty(0);
  }
}

int Motor::getPin(int type) {
  if (type == -1) {
    return pwm_pin_back;
  } else if (type == 1) {
    return pwm_pin_front;
  } else if (type == -2) {
    return high_pin_back;
  } else if (type ==2) {
    return high_pin_front;
  }
  else {
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

void Motor::setAggression(int new_a) {
  a = new_a;
}

void Motor::setFadeTime(int new_fade_time) {
  fade_time = new_fade_time;
}

void Motor::setLcTime(int new_lc_time) {
  T = new_lc_time;
}

void Motor::setZero() {
  ledcWrite(pwm_pin_front, 0);
  digitalWrite(high_pin_front, LOW);
  ledcWrite(pwm_pin_back, 0);
  digitalWrite(high_pin_back, LOW);
  current_duty = 0;
}

void Motor::startSafetyDelay() {
  is_blocking = true;
  delay_start_time = millis();
  setZero();
}

void Motor::startFade(int target_duty)
{
  is_fading = true;
  fade_target_duty = target_duty;
  fade_start_time = millis();
  fade_start_duty = current_duty;
  fade_ticker.attach_ms(fade_ticker_timing, [this]() { this->fading(); });
}

void Motor::fading()
{
  fade_current_time = millis();
  int elapsed = fade_current_time - fade_start_time;

  if(elapsed >= fade_time) {
    setDuty(fade_target_duty);
    stopFading();
  } else {
    int new_duty = fade_start_duty + (fade_target_duty - fade_start_duty) * elapsed / fade_time;
    setDuty(new_duty);
  }
}

void Motor::stopFading()
{
  is_fading = false;
  fade_ticker.detach();
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

void Motor::setDuty(int target_duty) {
  target_duty = checkDutyRange(target_duty);

  // Prüfe auf Richtungswechsel über die Null
  if ((target_duty > 0 && current_duty < 0) || (target_duty < 0 && current_duty > 0)) {
    ledcWrite(pwm_pin_front, 0);
    digitalWrite(high_pin_front, LOW);
    ledcWrite(pwm_pin_back, 0);
    digitalWrite(high_pin_back, LOW);
    current_duty = 0;
    startSafetyDelay();
    return;
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
    current_duty = 0;
    startSafetyDelay();
    return;
  }
}

void Motor::changeSpeed(int direction_vector) {
  int target_duty = current_duty + direction_vector;

  last_duty = current_duty;

  setDuty(target_duty);
}

void Motor::initFading() {
  should_fade = true;
}

void Motor::uninitFading() {
  should_fade = false;
}

void Motor::changeSpeedAbsolute(int target_duty)
{
  target_duty = checkDutyRange(target_duty);

  if(is_blocking) {
    if(millis() - delay_start_time <= direction_change_delay) {
      return;
    } else {
      is_blocking = false;
    }
  }

  if(is_launching) {
    if(target_duty >= 90)
    {
      return;
    }
    else
    {
      stopLaunchControl();
    }
  } else if(is_fading) {
    if(abs(fade_target_duty - target_duty) < break_fade_threshhold) {
      return;
    } else {
      stopFading();
    }
  } else if(abs(target_duty - current_duty) <= 3) {
    return;
  } else if( abs(target_duty - current_duty) >= threshold && should_fade) {
    startFade(target_duty);
  } else {
    setDuty(target_duty);
  }
}

int Motor::lcFunction(int t)
{
  //k = a*2*ln(p/1-p)/T
  //f(t) = L/1+e^-k(t-t0)
  //d(t) = 100/1+e^-(a*2*ln(0.99/1-0.99)/T)(t-T/2)
  T = 700; //in ms
  a = 2;

  float d = 100.0 / (1.0 + exp(-(a * 2.0 * log(0.99 / (1.0 - 0.99)) / T) * (t - T / 2.0)));
  return d;
}

bool Motor::stopLaunchControl()
{
  lc_ticker.detach();
  is_launching = false;
  return true;
}

bool Motor::launchControl()
{
  lc_current_time = millis();
  int target_duty = lcFunction(lc_current_time - lc_start_time);
  if(current_duty < 0)
  {
    return stopLaunchControl();
  }
  else if(current_duty < 95)
  {
    setDuty(target_duty);
    return true;
  }
  else
  {
    setDuty(100);
    return stopLaunchControl();
  }
}

bool Motor::startLaunchControl()
{
  setDuty(0);
  lc_start_time = millis();
  is_launching = true;
  lc_ticker.attach_ms(3, [this]() { this->launchControl(); });
  return true;
}
