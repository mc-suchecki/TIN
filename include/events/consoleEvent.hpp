#pragma once

#include <iostream>
#include "event.hpp"

class ConsoleEvent : public Event {
  public:
    ConsoleEvent(std::string message = "") : message(message) {
    }
    virtual ~ConsoleEvent() {};
    std::string getMessage() {return message;}
  private:
    std::string message;
};

class CreateConnectionEvent : public ConsoleEvent {
  public:
    CreateConnectionEvent(std::string address):address(address) {};
    virtual ~CreateConnectionEvent() {};
    std::string getAddress() {return address;};
  private:
    std::string address;
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
