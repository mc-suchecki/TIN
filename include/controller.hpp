#pragma once

#include "eventQueue.hpp"
#include "console.hpp"
#include <map>

/**
 * Application controller from MVC pattern - responsible for 
 * proccessing events from EventsQueue class coordinating the
 * communication within whole application.
 */
class Controller {
  typedef void (Controller::*MethodPointer)(Event *);

  public:
    Controller();
    ~Controller();

    void run();

  private:
    EventQueue *eventQueue;
    Console *console;

    //map converting Event types to actions handling them
    std::map<const std::type_info *, MethodPointer> eventActionMap;
    void fillEventActionMap();

    //methods handling particular events
    void createConnection(Event *event);
    void sendCommand(Event *event);
    void cancelAll(Event *event);

    //temporary method for testing puposes - TODO delete me!
    void handleConsoleEvent(Event *event);
};
