/** @author Jacek Witkowski
 *
 *  Definition of class representing connection with server. It is used
 *  by clients application.
 */

#pragma once

#include <string>
#include "eventQueue.hpp"

// Maybe I will make separate class for IPAddress and PortsNr in the future
typedef std::string IPAddress;
typedef unsigned short PortsNr;

/**
 * Class representing the connection with a remote server. Used for sending
 * commands and receiving results of their execution. Every instance of that
 * class represents a separate Server.
 */
class Connection {
  public:
    Connection(EventQueue * const evQ, IPAddress addr, PortsNr p):
      eventQueue(evQ), IP_ADDRESS(addr), PORTS_NUMBER(p) {}
    virtual ~Connection() {}

    void init();
    void execute(const std::string &command);
    void killAll();
    std::string getIPAddress();

  private:
    EventQueue * const eventQueue;
    const IPAddress IP_ADDRESS;
    const PortsNr PORTS_NUMBER;  
    char buffer[256]; //FIXME
    int sockfd;
};

