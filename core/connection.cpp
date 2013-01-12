/// @author Jacek Witkowski
///
/// Implementation of the Connection class.

#include <iostream>
#include <fstream>
#include <stdio.h>

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

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
    //Logger *logger = Logger::getInstance(cout);
    //logger->logDebugMessage("Created new connection object with address "+addr+" and port "+std::to_string(p));
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

void Connection::init(string password) {
  actionsQueue.push(new InitAction(this,password));
}

void Connection::execute(const Command &command) {
  actionsQueue.push(new ExecuteAction(this,command));
}

void Connection::close() {
  actionsQueue.push(new CloseAction(this));
}

void Connection::init_internal(string password) {
  if(!prepareSocket())
    return;

  if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
    string errorMsg = "Failed to connect to remote server";
    eventQueue->push(new ConnectionFailedEvent(errorMsg));
    return;
  }

  if(sendPassword(password))
    eventQueue->push(new ConnectionEstablishedEvent());

  else {
    string errorMsg = "Authentication failure";
    eventQueue->push(new ConnectionFailedEvent(errorMsg));
  }
}

void Connection::close_internal() {
  Command cmd("Sudden Death!", Command::CLOSE);

  if(!sendCommand(cmd))
    return;

  receiveResults();
}

void Connection::execute_internal(const Command &command) {
  if(sockfd < 0){
    cerr<< "(" << IP_ADDRESS << ") Cannot execute command on uninitialized connection" << endl;
    return;
  }

  if(!sendCommand(command))
    return;
}

std::string Connection::getIPAddress() {
  return IP_ADDRESS;
}

unsigned int Command::serialize(char *&serializedChunk_out) const {
  serializedChunk_out = new char[command.size()+1];
  strcpy(serializedChunk_out,command.c_str());

  return command.size()+1;
}

bool Connection::sendCommand(const Command &command) {
  char *serializedChunk;
  command.serialize(serializedChunk);
  string commandsContent(serializedChunk);

  strncpy(buffer, serializedChunk, BUFFER_SIZE); 
  int n = write(sockfd, buffer, strlen(buffer));

  if(n < 0) {
    string errMsg = "(" + IP_ADDRESS + ") Failed to write to socket";
    eventQueue->push(new CommandSendingFailedEvent(errMsg));
    cerr << errMsg << endl;
    return false;
  }

  eventQueue->push(new CommandSentEvent());
  return true;
}

void Connection::receiveResults() {
  //acquire current date and time
  char timeBuff[80];
  getCurrTime(timeBuff,80);

  // open a file of name IP_ADDRESS_DATE_TIME
  ofstream resultFile;
  string filename = IP_ADDRESS+"_"+timeBuff;
  resultFile.open(filename.c_str());

  // read in loop data and write them into file.
  int n;
  do {
    memset(buffer,0,BUFFER_SIZE);
    n = read(sockfd, buffer, BUFFER_SIZE-1);
    if(n<0)
      break; 

    resultFile.write(buffer, n);
    if(resultFile.fail()){
      cerr<<"Error while writing to the file"<<endl;
      n = -1;
    }
  }
  while(n>0);//FIXME How can I recognize the end of the transmission?

  if(n<0) {
    string errorMsg = "(" + IP_ADDRESS + ") Failed to receive the result file";
    eventQueue->push(new ReceivingResultsFailureEvent(errorMsg));
    cerr << errorMsg << std::endl;
  }
  else {
    ++numOfResults;
    eventQueue->push(new ActionDoneEvent(filename));
  }

  resultFile.close(); 
}

bool Connection::sendPassword(string password) {
  // FIXME md5(password) instead of plain-text password should be sent
  Command authCmd(password, Command::CLOSE);
  
  if(!sendCommand(authCmd))
    return false;

  //receive authentication response
  memset(buffer,0,BUFFER_SIZE);
  int n = read(sockfd, buffer, BUFFER_SIZE-1);
  if(n<0)
    return false; 

  if(strcmp(buffer,"ok") == 0)
    return true;

  return false;
}

bool Connection::prepareSocket(){
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    eventQueue->push(new ConnectionFailedEvent("Failed to open socket"));
    return false;
  }

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(PORTS_NUMBER);

  if(inet_pton(AF_INET, IP_ADDRESS.c_str(), &servAddr.sin_addr)<=0) {
    string errorMsg = "Failed to convert given IP "+IP_ADDRESS+" address to native type.";
    eventQueue->push(new ConnectionFailedEvent(errorMsg));
    return false;
  }

  return true;
}
