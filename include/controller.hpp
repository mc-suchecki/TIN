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
  public:
    Controller();
    ~Controller();

    void run();

  private:
    EventQueue *eventQueue;
    Console *console;

    //std::map<std::type_info, Action> eventActionMap;
};
