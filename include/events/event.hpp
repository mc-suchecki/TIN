#pragma once 

/** 
 * Abstract class, parent of ConsoleEvent and ConnectionEvent,
 * used to deal with the communication within the application.
 */
#include <string>

class Event {
  public:
    Event() {};
    virtual ~Event() {};
    virtual std::string getMessage(){return "This is an event";}
    //nothing to see here, move around
};
