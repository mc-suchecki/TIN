/// @author Jacek Witkowski
///
/// @brief Implementation of the Connection class.

#include <iostream>
#include <stdio.h>
#include <boost/thread.hpp>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/logger.hpp"
#include "../include/connection.hpp"
#include "../include/eventQueue.hpp"
#include "../include/events/connectionEvent.hpp"

using namespace std;

Connection::Connection(EventQueue * const evQ, IPAddress addr, PortsNr p):
  eventQueue(evQ), IP_ADDRESS(addr), PORTS_NUMBER(p){
  Logger * logger = Logger::getInstance(cout);
  logger->debug("Created new connection object with address "+addr+" and port "+std::to_string(p));
  stopLoop = false;
  runThread = boost::thread(&Connection::run, this);
  sockfd = -1;
}

Connection::~Connection() {
  stopLoop = true;
  runThread.join();
}

void Connection::run() {
  while(!stopLoop){
     Action *action = actionsQueue.pop();
     action->execute();
     delete action;
  }
}

void Connection::init() {
  actionsQueue.push(new InitAction(this));
}

void Connection::execute(const Command &command) {
  actionsQueue.push(new ExecuteAction(this,command));
}

void Connection::killAll() {
  actionsQueue.push(new KillAllAction(this));
}

void Connection::init_internal() {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    eventQueue->push(new ConnectionFailedEvent("Failed to open socket"));
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
    string errorMsg = "Failed to connect to remote server";
    eventQueue->push(new ConnectionFailedEvent(errorMsg));
    return;
  }

  eventQueue->push(new ConnectionEstablishedEvent());
}

void Connection::killAll_internal() {
  //TODO  
}

void Connection::execute_internal(const Command &command) {
  if(sockfd < 0)
    cerr<< "(" << IP_ADDRESS << ") Cannot execute command on uninitialized connection" << endl;

  strncpy(buffer, command.serialize(), 256);
  int n = write(sockfd, buffer, strlen(buffer));
  if(n < 0) {
    string errMsg = "(" + IP_ADDRESS + ") Failed to write to socket";
    eventQueue->push(new CommandSendingFailedEvent(errMsg));
    cerr << errMsg << endl;
    return;
  }

  eventQueue->push(new CommandSentEvent()); 
  //std::cout << "Command sent" << std::endl;

  memset(buffer,0,256);
  n = read(sockfd, buffer, 255);
  //std::cout << "server: started execution of tasks" << std::endl;
  if(n<0) {
    string errorMsg = "(" + IP_ADDRESS + ") Couldn't read data from socket";
    eventQueue->push(new ReceivingResultsFailureEvent(errorMsg));
    cerr << errorMsg << std::endl;
    return;
  }

  cout << buffer << endl;
  //TODO put read data into eventQueue
}

std::string Connection::getIPAddress() {
  return IP_ADDRESS;
}

const char * Command::serialize() const{
  char *serializedContent = new char[command.size()+1];
  strcpy(serializedContent,command.c_str());

  return serializedContent;
}

