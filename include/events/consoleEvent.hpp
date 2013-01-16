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
class CloseEvent : public ConsoleEvent{
  public:
    CloseEvent(std::string address = "")
      : ConsoleEvent("Close connection with " + address),
      address(address) {};
    std::string getAddress() {return address;}
    std::string getMessage() {return "Closing connection";}
  private:
    std::string address;
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

class GetFileEvent : public ConsoleEvent {
  public:
    GetFileEvent(std::string address, std::string remotePath, std::string localPath)
      : ConsoleEvent("Get file " + remotePath + " to " + localPath + "."),
       address(address), remotePath(remotePath), localPath(localPath) {};
    virtual ~GetFileEvent() {};
    virtual std::string getMessage() {return message;};
    std::string getAddress() {return address;};
    std::string getRemotePath() {return remotePath;};
    std::string getLocalPath() {return localPath;};
  private:
    std::string address;
    std::string remotePath;
    std::string localPath;
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
