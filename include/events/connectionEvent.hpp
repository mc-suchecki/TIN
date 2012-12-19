#pragma once

#include <iostream>
#include "event.hpp"

class ConnectionEvent : public Event {
  public:
    ConnectionEvent(std::string message) : message(message) {}
    virtual ~ConnectionEvent() {};
    std::string getMessage() {return message;}
  protected:
    std::string message;
};

class ConnectionEstablishedEvent: public ConnectionEvent {
  public:
    ConnectionEstablishedEvent():
      ConnectionEvent("connection established successfully") {}
    virtual ~ConnectionEstablishedEvent() {};
};

class ConnectionFailedEvent: public ConnectionEvent {
  public:
    ConnectionFailedEvent(const std::string &errorMsg = "") : ConnectionEvent(errorMsg) {};
    virtual ~ConnectionFailedEvent() {};
};

class CommandSentEvent: public ConnectionEvent {
  public:
    CommandSentEvent(): ConnectionEvent("Command sent successfully") {}
    virtual ~CommandSentEvent() {}
};

class CommandSendingFailedEvent: public ConnectionEvent {
  public:
    CommandSendingFailedEvent(const std::string &errorMsg = "") :
      ConnectionEvent(errorMsg) {}
    virtual ~CommandSendingFailedEvent() {};
};

class ActionDoneEvent : public ConnectionEvent {
  public:
    ActionDoneEvent(void *cont):
      ConnectionEvent("All commands completed"), content(cont) {};
    virtual ~ActionDoneEvent() {};
    void *content; //FIXME change it to file's path or sth
};

class ReceivingResultsFailureEvent: public ConnectionEvent {
  public:
    ReceivingResultsFailureEvent(const std::string &errorMsg = "") :
      ConnectionEvent(errorMsg) {}
    virtual ~ReceivingResultsFailureEvent() {}
};
