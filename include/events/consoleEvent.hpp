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
