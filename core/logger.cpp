#include "../include/logger.hpp"
#include "../include/events/event.hpp"
#include <iostream>
#include <typeinfo>

using namespace std;

Logger::Logger(ostream &out) : out(out) {
}

void Logger::logEvent(Event *event) {
  out << "[Logger] " 
    << typeid(*event).name() << ": "
    << event->getMessage() << endl;
}
