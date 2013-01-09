#pragma once

#include <iostream>
#include <boost/filesystem/path.hpp>
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
      ConnectionEvent("Connection established successfully.") {}
    virtual ~ConnectionEstablishedEvent() {};
};

class ConnectionFailedEvent: public ConnectionEvent {
  public:
    ConnectionFailedEvent(const std::string &errorMsg = "") : ConnectionEvent(errorMsg) {};
    virtual ~ConnectionFailedEvent() {};
};

class CommandSentEvent: public ConnectionEvent {
  public:
    CommandSentEvent(): ConnectionEvent("Command sent successfully.") {}
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
    ActionDoneEvent(const std::string &msg = ""):
      ConnectionEvent(msg) {}
    virtual ~ActionDoneEvent() {};
};

class ReceivingResultsFailureEvent: public ConnectionEvent {
  public:
    ReceivingResultsFailureEvent(const std::string &errorMsg = "") :
      ConnectionEvent(errorMsg) {}
    virtual ~ReceivingResultsFailureEvent() {}
};
