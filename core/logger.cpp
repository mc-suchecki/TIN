#include "../include/logger.hpp"
#include "../include/events/event.hpp"
#include <iostream>
#include <typeinfo>

using namespace std;

Logger::Logger(ostream &out) : out(out) {
}

void Logger::logEvent(Event *event) {
  //acquire current date and time
  char buffer[80];
  time_t now = time(0);
  tm* localtm = localtime(&now);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", localtm);

  out << "[" << buffer << "] " 
      << typeid(*event).name() << ": "
      << event->getMessage() << endl;
}
