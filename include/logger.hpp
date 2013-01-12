#pragma once

#include <string>
#include <ostream>
#include <fstream>

class Event;
class Config;

/**
 * Abstract class representing logging utility.
 */
class Logger {
  public:
    virtual void logEvent(Event *) = 0;
    virtual void logDebugMessage(std::string) = 0;
    virtual ~Logger() {};
  protected:
    const std::string getCurrentDateAndTime();
    Config *config;
};

/**
 * Class representing logging utility which displays log messages on screen.
 */
class ConsoleLogger : public Logger {
  public:
    ConsoleLogger(std::ostream &out);
    virtual ~ConsoleLogger() {};
    virtual void logEvent(Event *);
    virtual void logDebugMessage(std::string);
  private:
    std::ostream &out;
};

/**
 * Class representing logging utility which logs events to a file.
 */
class FileLogger : public Logger {
  public:
    FileLogger();
    virtual ~FileLogger();
    virtual void logEvent(Event *);
    virtual void logDebugMessage(std::string);
  private:
    std::ofstream logFile;
};
