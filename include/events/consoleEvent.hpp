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
    CreateConnectionEvent(std::string address = "", int port = 0, std::string alias = "",  std::string password = "")
      : address(address), port(port), alias(alias), password(password) {};
    virtual ~CreateConnectionEvent() {};
    virtual std::string getMessage() {
      if(port == 0)
        return "Connect to " + address + " on default port.";
      else
        return "Connect to " + address + " on port " + std::to_string(port) + ".";
    }

    std::string getAddress() {return address;};
    int getPort() {return port;};
    std::string getAlias(){ return alias;}
    std::string getPassword() {return password;}
  private:
    std::string address;
    int port;
    std::string alias;
    std::string password;
};
class CloseEvent : public ConsoleEvent{
  public:
    CloseEvent(std::string alias = "")
      : ConsoleEvent("Close connection with " + alias),
      alias(alias) {};
    std::string getAlias() {return alias;}
    std::string getMessage() {return "Closing connection";}
  private:
    std::string alias;
};

class SendCommandEvent : public ConsoleEvent {
  public:
    SendCommandEvent(std::string alias = "", std::string command = "")
      : ConsoleEvent("Send to " + alias + " command: " + command + "."),
        alias(alias), command(command) {};
    virtual ~SendCommandEvent() {};
    virtual std::string getMessage() {return message;};
    std::string getAlias() {return alias;};
    std::string getCommand() {return command;};
  private:
    std::string alias;
    std::string command;
};

class GetFileEvent : public ConsoleEvent {
  public:
    GetFileEvent(std::string alias, std::string remotePath, std::string localPath)
      : ConsoleEvent("Get file " + remotePath + " to " + localPath + "."),
       alias(alias), remotePath(remotePath), localPath(localPath) {};
    virtual ~GetFileEvent() {};
    virtual std::string getMessage() {return message;};
    std::string getAlias() {return alias;};
    std::string getRemotePath() {return remotePath;};
    std::string getLocalPath() {return localPath;};
  private:
    std::string alias;
    std::string remotePath;
    std::string localPath;
};

class CancelAllEvent : public ConsoleEvent {
  public:
    CancelAllEvent(std::string alias = "")
      : ConsoleEvent("Cancel all commands on " + alias + "."),
      alias(alias) {};
    virtual ~CancelAllEvent() {};
    virtual std::string getMessage() {return message;};
    std::string getAlias() {return alias;};
  private:
    std::string alias;
};
