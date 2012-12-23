#pragma once

/**
 * Simple logging utility.
 */

#include <ostream>

class Event;

class Logger {
  public:
    Logger(std::ostream& out);
    virtual ~Logger(){};
    void logEvent(Event *);
  private:
    std::ostream & out;
};
