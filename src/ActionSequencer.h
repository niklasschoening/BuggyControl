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
  void execute(int i);
  void attachTicker();
  void detachTicker();

  std::vector<std::function<void()>> scheduled_functions;
  std::vector<int> scheduled_delays;
  std::vector<int> start_times;
  std::vector<int> repetitions;

  Ticker schedule_checker;
  int schedule_checker_timing;

public:
  ActionSequencer(int timing);

  // Deine Methoden hier
  void addSq(std::function<void()> function, int delay, int reps);
  void clearSq();

};

#endif // ACTION_SEQUENCER_H
