#include "ActionSequencer.h"

ActionSequencer::ActionSequencer(int timing) {
  // Deine Initialisierung hier
  scheduled_functions = {};
  scheduled_delays = {};
  start_times = {};
  repetitions = {};

  schedule_checker_timing = timing;
}

void ActionSequencer::checkSchedule() {
  for(size_t i = 0; i < scheduled_delays.size(); i++) {
    if(scheduled_delays[i] <= millis() - start_times[i]) {
      execute(i);
      i--;
    }
  }

  if(scheduled_delays.size() == 0) {
    detachTicker();
  }
}

void ActionSequencer::execute(int i) {
  scheduled_functions[i]();
  if(repetitions[i] <= 1) {
    scheduled_delays.erase(scheduled_delays.begin() + i);
    scheduled_functions.erase(scheduled_functions.begin() + i);
    start_times.erase(start_times.begin() + i);
    repetitions.erase(repetitions.begin() + i);
  }
  else {
    start_times[i] = millis();
    repetitions[i] --;
  }
      

}

void ActionSequencer::attachTicker() {
  schedule_checker.attach_ms(schedule_checker_timing, [this]() { this->checkSchedule(); });
}

void ActionSequencer::detachTicker() {
  schedule_checker.detach();
}

void ActionSequencer::addSq(std::function<void()> function, int delay, int reps) {
  if(reps <= 0) {
    return;
  }
  if(scheduled_functions.size() == 0)
  {
    attachTicker();
  }
  scheduled_functions.push_back(function);
  scheduled_delays.push_back(delay);
  start_times.push_back(millis());
  repetitions.push_back(reps);
}

void ActionSequencer::clearSq() {
  scheduled_functions.clear();
  scheduled_delays.clear();
  start_times.clear();
  repetitions.clear();
}