#pragma once

/**
 * Simple logging utility.
 */

#include <ostream>
#include <string>

class Event;
class Config;

class Logger {
  public:
    static Logger * getInstance(std::ostream& out);
    void logEvent(Event *);
    void debug(std::string);
  private:
    Logger(std::ostream& out);
    Logger& operator= (Logger&);
    Logger(Logger&);
    ~Logger(){};

    static Logger * instance;
    std::ostream & out;
    Config * config;
};
