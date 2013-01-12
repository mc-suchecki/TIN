#include "../include/logger.hpp"
#include "../include/events/event.hpp"
#include "../include/config.hpp"
#include <iostream>
#include <typeinfo>

using namespace std;

/** Method for acquiring string with current date and time. */
const std::string Logger::getCurrentDateAndTime() {
  char buffer[80];
  time_t now = time(0);
  tm* localtm = localtime(&now);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", localtm);
  return buffer;
}

/** ConsoleLogger constructor - initialises variables. */
ConsoleLogger::ConsoleLogger(ostream &out) : out(out) {
  config = Config::getInstance();
}

/** Method for logging Events on the screen. */
void ConsoleLogger::logEvent(Event *event) {
  if(config->getDebug() > 0)
    out << "[" << getCurrentDateAndTime() << "] " 
        << typeid(*event).name() << ": "
        << event->getMessage() << endl;
}

/** Method for displaying debug messages on the screen. */
void ConsoleLogger::logDebugMessage(string message) {
  if(config->getDebug() > 1)
    out << message << endl;
}

/** FileLogger constructor - initialises variables. */
FileLogger::FileLogger() {
  config = Config::getInstance();
  logFile.open(config->getLogFile());
}

/** FileLogger destructor - closes the log file. */
FileLogger::~FileLogger() {
  logFile.close();
}

/** Method for logging Events in log file. */
void FileLogger::logEvent(Event *event) {
  if(config->getDebug() > 0)
    logFile << "[" << getCurrentDateAndTime() << "] " 
            << typeid(*event).name() << ": "
            << event->getMessage() << endl;
}

/** Method for writing debug messages to log file. */
void FileLogger::logDebugMessage(string message) {
  if(config->getDebug() > 1)
    logFile << message << endl;
}
