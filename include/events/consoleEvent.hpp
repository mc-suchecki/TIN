#pragma once

#include <iostream>
#include <stdlib.h>
#include "event.hpp"
#include <string>
#include <sstream>

class ConsoleEvent : public Event {
  public:
    ConsoleEvent(std::string message = "") : message(message) {
    }
    virtual ~ConsoleEvent() {};
    virtual std::string getMessage() {return "This is a console event";}
  private:
    std::string message;
};

class CreateConnectionEvent : public ConsoleEvent {
  public:
    CreateConnectionEvent(std::string address,
        int port = 0):address(address),port(port) {};
    virtual ~CreateConnectionEvent() {};
    std::string getAddress() {return address;};
    int getPort() {return port;};
    virtual std::string getMessage() {
      std::stringstream out;
      out << port;
      return "Connect to "+address+" on port "+out.str();}
  private:
    std::string address;
    int port;
};

class SendCommandEvent : public ConsoleEvent {
  public:
    SendCommandEvent() {};
    virtual ~SendCommandEvent() {};
};

class CancelAllEvent : public ConsoleEvent {
  public:
    CancelAllEvent() {};
    virtual ~CancelAllEvent() {};
};
