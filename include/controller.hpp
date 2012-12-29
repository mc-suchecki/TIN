#pragma once

#include "eventQueue.hpp"
#include "connection.hpp"
#include "console.hpp"
#include "logger.hpp"
#include <map>

class Config;

/**
 * Application controller from MVC pattern - responsible for 
 * proccessing events from EventsQueue class coordinating the
 * communication within the whole application.
 */
class Controller {
  typedef void (Controller::*MethodPointer)(Event *);

  public:
    Controller(int argc, char * argv[]);
    ~Controller();

    //runs constantly and process events
    void run();

  private:
    //connections with other objects
    EventQueue *eventQueue;
    Console *console;
    Logger *logger;
    Config *config;

    //vector of active connections
    std::vector<Connection *> activeConnections;

    //configuration handling
    bool handleConfig(int argc, char * argv[]);

    //map converting Event types to actions handling them
    std::map<const std::type_info *, MethodPointer> eventActionMap;
    void fillEventActionMap();

    //methods handling particular console events
    void createConnection(Event *event);
    void sendCommand(Event *event);
    void cancelAll(Event *event);

    //methods handling particular connection events
    void logMessage(Event *event);
    void saveResults(Event *event);
};
