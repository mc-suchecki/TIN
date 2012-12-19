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
    CreateConnectionEvent() {};
    virtual ~CreateConnectionEvent() {};
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
