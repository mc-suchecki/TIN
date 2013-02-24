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
#include "../common/MessageDictionary.hpp"
#include "../common/MD5.hpp"

using namespace std;

Connection::Connection(EventQueue * const evQ, IPAddress addr, PortsNr p):
  eventQueue(evQ), IP_ADDRESS(addr), PORTS_NUMBER(p){
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

void Connection::downloadFile(string remoteFile, string localFile){
  actionsQueue.push(new DownloadFileAction(this, remoteFile, localFile));
}

void Connection::close() {
  actionsQueue.push(new CloseAction(this));
}

void Connection::init_internal(string password) {
  if(!prepareSocket())
    return;

  if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
    string errorMsg = "Failed to connect to remote server";
    eventQueue->push(new ConnectionFailedEvent(IP_ADDRESS, errorMsg));
    return;
  }

  if(authenticate(password))
    eventQueue->push(new ConnectionEstablishedEvent(IP_ADDRESS));

  else {
    string errorMsg = "Authentication failure";
    eventQueue->push(new ConnectionFailedEvent(IP_ADDRESS, errorMsg));
    sockfd = -1;
  }
}

void Connection::close_internal() {
  memset(buffer, 0, BUFFER_SIZE);
  strncpy(buffer, MessageDictionary::closeConnection.c_str(), BUFFER_SIZE); 
  int n = write(sockfd, buffer, strlen(buffer));

  if(n < 0) {
    string errorMsg = "(" + IP_ADDRESS
      + ") Failed to send CLOSE_CONNECTION command to server";

    eventQueue->push(new ConnectionFailedEvent(IP_ADDRESS, errorMsg));
  }
}

void Connection::execute_internal(const Command &command) {
  if(sockfd < 0){
    string errMsg = "(" + IP_ADDRESS + ") Cannot execute command on uninitialized connection";
    eventQueue->push(new CommandSendingFailedEvent(IP_ADDRESS, errMsg));
    return;
  }

  if(!sendCommand(command))
    return;
}

void Connection::downloadFile_internal(string remoteFile, string localFile){
  //send request for a file
  string fileRequest = MessageDictionary::sendFile;
  fileRequest += remoteFile;

  memset(buffer, 0, BUFFER_SIZE);
  strncpy(buffer, fileRequest.c_str(), BUFFER_SIZE); 
  int n = write(sockfd, buffer, strlen(buffer));
  if(n<=0){
    string errMsg = "(" + IP_ADDRESS + ") Failed to send request for a file";
    eventQueue->push(new CommandSendingFailedEvent(IP_ADDRESS, errMsg));
    return;
  }

  //receive file
  if(!receiveAndSaveFile(localFile)){
    string errMsg = "(" + IP_ADDRESS + ") Failed to receive file";
    eventQueue->push(new CommandSendingFailedEvent(IP_ADDRESS, errMsg));
  }
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

  memset(buffer,0,BUFFER_SIZE);
  strncpy(buffer, serializedChunk, BUFFER_SIZE); 
  int n = write(sockfd, buffer, strlen(buffer));

  if(n < 0) {
    string errMsg = "(" + IP_ADDRESS + ") Failed to write to socket";
    eventQueue->push(new CommandSendingFailedEvent(IP_ADDRESS, errMsg));
    return false;
  }

  eventQueue->push(new CommandSentEvent(IP_ADDRESS));
  return true;
}

bool Connection::sendAuthenticationInfo(string password, string challenge){
  string challPass = password;
  challPass += challenge;
  challPass = md5(challPass);

  memset(buffer, 0, BUFFER_SIZE);
  strncpy(buffer, challPass.c_str(), BUFFER_SIZE); 
  int n = write(sockfd, buffer, strlen(buffer));

  if(n < 0) {
    string errorMsg = "(" + IP_ADDRESS + ") Failed to write password to socket";
    eventQueue->push(new ConnectionFailedEvent(IP_ADDRESS, errorMsg));
    return false;
  }

  return true;
}

bool Connection::authenticate(string password) {
  std::string challenge = getChallenge();
  if(challenge.compare("0") == 0)
    return false;

  if(!sendAuthenticationInfo(password,challenge))
    return false;

  //receive authentication response
  int bytesReceived = receiveMsg();
  if(bytesReceived<0)
    return false; 

  if(strcmp(buffer,MessageDictionary::passwordCorrect.c_str()) == 0)
    return true;

  return false;
}

bool Connection::prepareSocket(){
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    eventQueue->push(new ConnectionFailedEvent(IP_ADDRESS, "Failed to open socket"));
    return false;
  }

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(PORTS_NUMBER);

  if(inet_pton(AF_INET, IP_ADDRESS.c_str(), &servAddr.sin_addr)<=0) {
    string errorMsg = "Failed to convert given IP "+IP_ADDRESS+" address to native type.";
    eventQueue->push(new ConnectionFailedEvent(IP_ADDRESS, errorMsg));
    return false;
  }

  return true;
}

int Connection::receiveMsg(){
  memset(buffer, 0, BUFFER_SIZE);
  int bytesRead = read(sockfd, buffer, BUFFER_SIZE-1);
  return bytesRead;
}

void Connection::getCurrTime(char *timeBuff, int n) {
  time_t now = time(0);
  tm *localtm = localtime(&now);
  strftime(timeBuff, n*sizeof(char), "%Y-%m-%d_%X", localtm);
}

bool Connection::receiveAndSaveFile(string localFile){
  ofstream resultFile;
  resultFile.open(localFile.c_str());

  //receive file's size
  int bytesRead = receiveMsg();
  if(bytesRead <= 0){
    string errorMsg = "(" + IP_ADDRESS + ") Failed to receive the result file's size";
    eventQueue->push(new ReceivingResultsFailureEvent(IP_ADDRESS, errorMsg));
    return false;
  }

  //i assumed that fileSize (4 bytes) will be sent in one package (won't be split).
  unsigned int fileSize = *((int *) buffer),
               remainingBytes = fileSize;

  resultFile.write(buffer+sizeof(fileSize), bytesRead - sizeof(fileSize));
  remainingBytes -= bytesRead-sizeof(fileSize);

  if(resultFile.fail()){
    string errorMsg = "(" + IP_ADDRESS + ") Failed to save received chunk of file";
    eventQueue->push(new ReceivingResultsFailureEvent(IP_ADDRESS, errorMsg));
    return false;
  }

  while(remainingBytes > 0){
    int bytesRead = receiveMsg();

    if(bytesRead <= 0){
      string errorMsg = "(" + IP_ADDRESS + ") Failed to receive the result file";
      eventQueue->push(new ReceivingResultsFailureEvent(IP_ADDRESS, errorMsg));
      return false;
    }

    resultFile.write(buffer, bytesRead);
    remainingBytes -= bytesRead;

    if(resultFile.fail()){
      string errorMsg = "(" + IP_ADDRESS + ") Failed to save received file";
      eventQueue->push(new ReceivingResultsFailureEvent(IP_ADDRESS, errorMsg));
      return false;
    }
  }

  eventQueue->push(new ActionDoneEvent(IP_ADDRESS, localFile));

  resultFile.close();
  return true;
}

string Connection::getChallenge(){
  //send request for challenge
  memset(buffer, 0, BUFFER_SIZE);
  strncpy(buffer, MessageDictionary::sendChallenge.c_str(), BUFFER_SIZE); 
  int bytesWritten = write(sockfd, buffer, strlen(buffer));
  if(bytesWritten<=0)
    return string("0");
 
  //receive challenge
  int bytesReceived = receiveMsg();
  if(bytesReceived<0)
    return string("0");
    
  return string(buffer);
}
