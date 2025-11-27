#ifndef ACTION_SEQUENCER_H
#define ACTION_SEQUENCER_H

#include <Arduino.h>
#include <functional>
#include <vector>
#include <Ticker.h>

class ActionSequencer {
private:
  // Deine private Variablen hier
  void checkSchedule();
  void execute();
  void attachTicker();
  void detachTicker();

  std::vector<std::function<void()>> scheduled_functions;
  std::vector<int> scheduled_delays;

  Ticker schedule_checker;
  int schedule_checker_timing;

public:
  ActionSequencer();

  // Deine Methoden hier
  void addSq(std::function<void()> function, int delay);
  void clearSq();

};

#endif // ACTION_SEQUENCER_H
