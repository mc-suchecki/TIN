/** @author Jacek Witkowski
 *
 *  Definition of class representing connection with server. It is used
 *  by clients application.
 */

#pragma once

#include <string>
#include "eventQueue.hpp"

// Maybe I will make separate class for IPAddress and PortsNr in the future
typedef IPAddress std::string;
typedef PortsNr unsigned short;

/**
 * Class representing the connection with a remote server. Used for sending
 * commands and receiving results of their execution. Every instance of that
 * class represents a separate Server.
 */
class Connection {
  public:
    Connection(EventQueue * const eventQueue, IPAddress addr, PortsNr p):
      IP_ADDRESS(addr), PORTS_NUBMER(p) {}
    virtual ~Connection() {}

    void init();
    void execute(std::string &command);
    void killAll();
    std::string getResults(); //FIXME what should be the return type?

  private:
    const IPAddress IP_ADDRESS;
    const PortsNr PORTS_NUBMER;  
    char buffer[256]; //FIXME

};
