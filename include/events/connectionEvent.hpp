#pragma once

#include <iostream>
#include <boost/filesystem/path.hpp>
#include "event.hpp"

class ConnectionEvent : public Event {
  public:
    ConnectionEvent(const std::string &connectionIPAddress,
        const std::string &message) : address(connectionIPAddress), message(message) {}
    virtual ~ConnectionEvent() {};
    std::string getMessage() {return message;}
    std::string getIPAddress() {return address;}
  protected:
    std::string address;
    std::string message;
};

class ConnectionEstablishedEvent: public ConnectionEvent {
  public:
    ConnectionEstablishedEvent(const std::string &connectionIPAddress = "")
      : ConnectionEvent(connectionIPAddress, "Connection established successfully.") {}
    virtual ~ConnectionEstablishedEvent() {};
};

class ConnectionFailedEvent: public ConnectionEvent {
  public:
    ConnectionFailedEvent(const std::string &connectionIPAddress = "",
        const std::string &errorMsg = "") : ConnectionEvent(connectionIPAddress, errorMsg) {};
    virtual ~ConnectionFailedEvent() {};
};

class CommandSentEvent: public ConnectionEvent {
  public:
    CommandSentEvent(const std::string &connectionIPAddress = "")
      : ConnectionEvent(connectionIPAddress, "Command sent successfully.") {}
    virtual ~CommandSentEvent() {}
};

class CommandSendingFailedEvent: public ConnectionEvent {
  public:
    CommandSendingFailedEvent(const std::string &connectionIPAddress = "",
        const std::string &errorMsg = "") : ConnectionEvent(connectionIPAddress, errorMsg) {};
    virtual ~CommandSendingFailedEvent() {};
};

class ActionDoneEvent : public ConnectionEvent {
  public:
    ActionDoneEvent(const std::string &connectionIPAddress = "",
        const std::string &msg = "") : ConnectionEvent(connectionIPAddress, msg) {}
    virtual ~ActionDoneEvent() {};
};

class ReceivingResultsFailureEvent: public ConnectionEvent {
  public:
    ReceivingResultsFailureEvent(const std::string &connectionIPAddress = "",
        const std::string &errorMsg = "") : ConnectionEvent(connectionIPAddress, errorMsg) {};
    virtual ~ReceivingResultsFailureEvent() {}
};
