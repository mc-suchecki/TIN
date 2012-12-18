/// @author Jacek Witkowski
///
/// @brief Definiuje klasę Connection używaną przez Nadzorcę. Każda instancja tej klasy
/// reprezentuje osobny Serwer wykonujący polecenia Nadzorcy.

#include <sys/socket.h>

class Connection {
private:
  const IPAddress IP_ADDRESS;
  const PortsNr PORTS_NUBMER;  

public:
  Connection(IPAddress addr, PortsNr p):
    IP_ADDRESS(addr), PORTS_NUBMER(p) {}

  void init();
  void execute(std::string &command);
  void killAll;
};

void Connection::init() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    //TODO throw Exception;
  }

  // Do poprawienia
  struct sockaddr_in servAddr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portsNr);  

  if(inet_pton(AF_INET, IP_ADDRESS, &servAddr.sin_addr)<=0) {
    //TODO throw Exception;
  }

  if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
    //TODO throw Exception;
  }

}

void Connection::killAll() {
  //TODO
}

void Connection::execute(string &command) {
  //TODO
}