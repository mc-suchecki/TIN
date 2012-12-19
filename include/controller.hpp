#pragma once

#include "eventQueue.hpp"
#include "connection.hpp"
#include "console.hpp"
#include "logger.hpp"
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
    Logger *logger;

    //vector of active connections
    std::vector<Connection *> activeConnections;

    //map converting Event types to actions handling them
    std::map<const std::type_info *, MethodPointer> eventActionMap;
    void fillEventActionMap();

    //methods handling particular console events
    void createConnection(Event *event);
    void sendCommand(Event *event);
    void cancelAll(Event *event);

    //methods handling particular connection events
    void displayMessage(Event *event);

    //temporary method for testing puposes - TODO delete me!
    void handleConsoleEvent(Event *event);
};
