
#include "LEDManager.h"

LEDManager::LEDManager() : indicator_ticker() {
  // Default-Konstruktor für globale Initialisierung
  rest_state = 0;
  brightness = 0;
  freq = 0;
  indicating = false;
  indicator_state = false;
  indicator_timing = 600;
}

LEDManager::LEDManager(std::vector<int> _leds, int _rest_state, int _brightness, int _freq) : indicator_ticker() {
  leds = _leds;
  rest_state = _rest_state;
  brightness = _brightness;
  freq = _freq;
  indicating = false;
  indicator_state = false;
  indicator_timing = 600;

  for (int i : leds) {
    ledcAttach(i, freq, 8);
  }
  rest();
}

void LEDManager::rest() {
  for (int i : leds) {
    switch (rest_state) {
      case 0:
        // aus
        ledcWrite(i, 0);
        break;
      case 1:
        // an
        ledcWrite(i, brightness * 255 / 100);
        break;
      case 2:
        break;
    }
  }
}

void LEDManager::startIndicating() {
  if (indicating) {
    return;
  }
  indicating = true;
  indicator_ticker.attach_ms(indicator_timing, indicateCallback, this);
}

void LEDManager::stopIndicating() {
  if (!indicating) {
    return;
  }
  indicator_ticker.detach();
  indicating = false;
  rest();
}

void LEDManager::turnOn(int _brightness) {
  for (int i : leds) {
    ledcWrite(i, _brightness * 255 / 100);
  }
}

void LEDManager::turnOff() {
  for (int i : leds) {
    ledcWrite(i, 0);
  }
}

void LEDManager::setIndicatorTiming(int timing_ms) {
  indicator_timing = timing_ms;
}

void LEDManager::indicateCallback(LEDManager* instance) {
  instance->indicate();
}

void LEDManager::indicate() {
  indicator_state = !indicator_state;
  for (int i : leds) {
    if (indicator_state) {
      ledcWrite(i, brightness * 255 / 100);
    } else {
      ledcWrite(i, 0);
    }
  }
}
