/// @author Jacek Witkowski
///
/// @brief Definition of class representing connection with server. It is used
/// by clients application.

#include <string>

// Maybe I will make separate class for IPAddress and PortsNr in the future
typedef IPAddress unsigned int;
typedef PortsNr unsigned short;

/// Class representing the connection with a remote server. Used for sending
/// commands and receiving results of their execution. Every instance of that
/// class represents a separate Server.
class ClientsConnection {
  private:
    const IPAddress IP_ADDRESS;
    const PortsNr PORTS_NUBMER;  

  public:
    ClientsConnection(IPAddress addr, PortsNr p):
      IP_ADDRESS(addr), PORTS_NUBMER(p) {}

    void init();
    void execute(std::string &command);
    void killAll();
    std::string getResults(); //FIXME what should be the return type?
};
