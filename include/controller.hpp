#pragma once

#include "eventQueue.hpp"
#include "console.hpp"
//class Console;
//class EventQueue;

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
};
