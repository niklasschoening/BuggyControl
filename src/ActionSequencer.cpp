#include "ActionSequencer.h"

ActionSequencer::ActionSequencer() {
  // Deine Initialisierung hier
  scheduled_functions = {};
  scheduled_delays = {};
}

void ActionSequencer::checkSchedule() {
  for(int i in scheduled_functions) {

  }
}

void ActionSequencer::execute() {

}

void ActionSequencer::attachTicker() {
  schedule_checker.attach_ms(schedule_checker_timing, [this]() { this->checkSchedule(); });
}

void ActionSequencer::detachTicker() {

}

void ActionSequencer::addSq(std::function<void()> function, int delay) {
  if(scheduled_functions.size() == 0)
  {
    attachTicker();
  }
  scheduled_functions.push_back(function);
  scheduled_delays.push_back(delay);
}

void ActionSequencer::clearSq() {
  scheduled_functions.clear();
  scheduled_delays.clear();
}