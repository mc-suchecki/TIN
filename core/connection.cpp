/// @author Jacek Witkowski
///
/// @brief Implementation of the ClientsConnection class

#include <stdio.h>
#include <sys/socket.h>
#include "../include/connection.hpp"

using std::string;

void Connection::init() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    eventQueue->push(ConnectionFailedEvent("failed to open socket"));
    return;
  }

  struct sockaddr_in servAddr;
  memset(&servAddr, 0, sizeof(servAddr)); //TODO replace with generic fill()
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portsNr);

  if(inet_pton(AF_INET, IP_ADDRESS.c_str(), &servAddr.sin_addr)<=0) {
    eventQueue->push(
      ConnectionFailedEvent("failed to convert given IP address to native type"));
    return;
  }

  if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
    eventQueue->push(ConnectionFailedEvent("failed to connect to remote server"));
    return;
  }

  eventQueue->push(ConnectionEstablishedEvent());
}

void Connection::killAll() {
  
}

void Connection::execute(const string &command) {
  strncpy(command.c_str(&buffer, &(command.c_str()), 256));
  int n = write(sockfd, buffer, strlen(buffer));
  if(n < 0) {
    eventQueue->push(CommandSendingFailedEvent());
    return;
  }

  eventQueue->push(CommandSentEvent()); 

  n = read(sockfd, buffer, 255);
  if(n<0) {
    eventQueue->push(ReceivingResultsFailureEvent("Couldn't read data from socket"));
    return;
  }

  //TODO put read data into eventQueue
}
