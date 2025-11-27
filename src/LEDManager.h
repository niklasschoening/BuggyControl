#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>
#include <Ticker.h>
#include <vector>

class LEDManager {
private:
  std::vector<int> leds;  // Pins der verwalteten LEDs
  int rest_state;  // In welchem Zustand die LED standardmässig verbleiben sollte. 0 = aus, 1 = an
  int brightness;  // Maximale Helligkeit der LED zwischen 0 und 100
  int freq;        // Frequenz der verwalteten LEDs

  Ticker indicator_ticker;
  bool indicating;
  bool indicator_state;
  int indicator_timing;  // Blinker-Timing in ms

  static void indicateCallback(LEDManager* instance);
  void indicate();

public:
  LEDManager(std::vector<int> leds, int rest_state, int brightness, int freq);
  LEDManager();  // Default-Konstruktor

  void rest();
  void startIndicating();
  void stopIndicating();
  void turnOn(int brightness);
  void turnOff();

  void setIndicatorTiming(int timing_ms);
};

#endif // LED_MANAGER_H
