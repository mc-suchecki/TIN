#pragma once 

/** 
 * Abstract class, parent of ConsoleEvent and ConnectionEvent,
 * used to deal with the communication within the application.
 */
class Event {
  public:
    Event() {};
    virtual ~Event() {};
    //nothing to see here, move around
};
