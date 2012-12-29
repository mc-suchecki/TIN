#include "../include/logger.hpp"
#include "../include/events/event.hpp"
#include "../include/config.hpp"
#include <iostream>
#include <typeinfo>

using namespace std;

Logger * Logger::instance = nullptr;

/// singleton
Logger * Logger::getInstance(ostream & out){
  if(instance == nullptr){
    instance = new Logger(out);
  }
return instance;
}

void Logger::logEvent(Event *event) {
  //acquire current date and time
  char buffer[80];
  time_t now = time(0);
  tm* localtm = localtime(&now);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", localtm);
  if(config->getDebug() > 0) {
    out << "[" << buffer << "] " 
      << typeid(*event).name() << ": "
      << event->getMessage() << endl;
  }
}

void Logger::debug(string message){
  if(config->getDebug() > 1) {
    out << message << endl;
  }
}

Logger::Logger(ostream &out) : out(out) {
  config = Config::getInstance();
}
