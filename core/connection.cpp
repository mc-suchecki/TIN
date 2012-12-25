/// @author Jacek Witkowski
///
/// @brief Implementation of the ClientsConnection class.

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/connection.hpp"
#include "../include/eventQueue.hpp"
#include "../include/events/connectionEvent.hpp"

using std::string;
using std::cout;
using std::endl;

void Connection::init() {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    eventQueue->push(new ConnectionFailedEvent("Failed to open socket."));
    return;
  }

  struct sockaddr_in servAddr;
  memset(&servAddr, 0, sizeof(servAddr)); //TODO replace with generic fill()
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(PORTS_NUMBER);

  if(inet_pton(AF_INET, IP_ADDRESS.c_str(), &servAddr.sin_addr)<=0) {
    string errorMsg = "Failed to convert given IP "+IP_ADDRESS+" address to native type.";
    eventQueue->push(new ConnectionFailedEvent(errorMsg));
    return;
  }

  if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
    string errorMsg = "Failed to connect to remote server.";
    eventQueue->push(new ConnectionFailedEvent(errorMsg));
    return;
  }

  eventQueue->push(new ConnectionEstablishedEvent());
}

void Connection::killAll() {
  
}

void Connection::execute(const string &command) {
  strncpy(buffer, command.c_str(), 256);
  int n = write(sockfd, buffer, strlen(buffer));
  if(n < 0) {
    string errMsg = "Failed to write to socket.";
    eventQueue->push(new CommandSendingFailedEvent(errMsg));
    //std::cout << "Failed to write to socket" << std::endl;
    return;
  }

  eventQueue->push(new CommandSentEvent()); 
  //std::cout << "Command sent" << std::endl;

  memset(buffer,0,256);
  n = read(sockfd, buffer, 255);
  //std::cout << "server: started execution of tasks" << std::endl;
  if(n<0) {
    string errorMsg = "Couldn't read data from socket.";
    eventQueue->push(new ReceivingResultsFailureEvent(errorMsg));
    //std::cout << "Couldn't read from socket" << std::endl;
    return;
  }

  cout << buffer << endl;
  //TODO put read data into eventQueue
}

std::string Connection::getAddress() {
  return IP_ADDRESS;
}
