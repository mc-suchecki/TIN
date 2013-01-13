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
    virtual std::string getMessage() = 0;
  protected:
    std::string message;
};

class CreateConnectionEvent : public ConsoleEvent {
  public:
    CreateConnectionEvent(std::string address = "", int port = 0, std::string password = "")
      : address(address), port(port), password(password) {};
    virtual ~CreateConnectionEvent() {};
    virtual std::string getMessage() {
      if(port == 0)
        return "Connect to " + address + " on default port.";
      else
        return "Connect to " + address + " on port " + std::to_string(port) + ".";
    }

    std::string getAddress() {return address;};
    int getPort() {return port;};
    std::string getPassword() {return password;}
  private:
    std::string address;
    int port;
    std::string password;
};

class SendCommandEvent : public ConsoleEvent {
  public:
    SendCommandEvent(std::string address = "", std::string command = "")
      : ConsoleEvent("Send to " + address + " command: " + command + "."),
        address(address), command(command) {};
    virtual ~SendCommandEvent() {};
    virtual std::string getMessage() {return message;};
    std::string getAddress() {return address;};
    std::string getCommand() {return command;};
  private:
    std::string address;
    std::string command;
};

class CancelAllEvent : public ConsoleEvent {
  public:
    CancelAllEvent(std::string address = "")
      : ConsoleEvent("Cancel all commands on " + address + "."),
      address(address) {};
    virtual ~CancelAllEvent() {};
    virtual std::string getMessage() {return message;};
    std::string getAddress() {return address;};
  private:
    std::string address;
};
