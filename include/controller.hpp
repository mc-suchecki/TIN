#pragma once

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
};
