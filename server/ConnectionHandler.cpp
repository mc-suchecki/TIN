<<<<<<< HEAD
/*
 * ConnectionHandler.cpp
 *
 *  Created on: 26-12-2012
 *      Author: przemek
 */
=======
#include <iostream>
#include "ConnectionHandler.h"
//#include <boost/thread.hpp>
>>>>>>> upstream/master

#include "ConnectionHandler.hpp"
#include <fstream>

ConnectionHandler::ConnectionHandler(int port,
		  BlockingQueue<std::string/*Command*/> * commandQueue,
		  BlockingQueue<std::string> * resultFileQueue)
{
	this->commandQueue = commandQueue;
	this->resultFileQueue = resultFileQueue;
	portNumber = port;

	initializeMySocket();
}

void ConnectionHandler::initializeMySocket()
{
<<<<<<< HEAD
	mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mySocket < 0)
		error("ERROR opening socket");
	bzero((char *) &serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portNumber);
	if (bind(mySocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
		error("ERROR on binding");
=======
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");
  bzero((char *) &servAddr, sizeof(servAddr));

  portno = port;

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  servAddr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    error("ERROR on binding");
>>>>>>> upstream/master
}

ConnectionHandler::~ConnectionHandler()
{
<<<<<<< HEAD
    close(mySocket);
=======
  close(sockfd);
>>>>>>> upstream/master
}

void ConnectionHandler::start()
{
<<<<<<< HEAD
    connectionHandler = boost::thread(&ConnectionHandler::watchForClientRequests, this);
=======
  startCommunication();
  //communicationHandler = boost::thread(&ConnectionHandler::StartCommunication, this);
>>>>>>> upstream/master
}

void ConnectionHandler::join()
{
<<<<<<< HEAD
    connectionHandler.join();
=======
  //communicationHandler.join();
>>>>>>> upstream/master
}

//i assume, that only one command is received before it's results are sent back
void ConnectionHandler::watchForClientRequests()
{
<<<<<<< HEAD
    while(true)
    {
    	initializeConnection();
    	receiveMessage();
    	sendResults();
    	closeConnection();
=======
  int n;
  while(true) {
    listen(sockfd,5);
    clilen = sizeof(cliAddr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cliAddr, &clilen);
    if (newsockfd < 0)
      error("ERROR on accept");

    bzero(buffer,256);
    while((n = read(newsockfd,buffer,255)))
    {
      if(n < 0)
        error("ERROR reading from socket");

      std::cout << "Here is the message:" << buffer << std::endl;
      n = write(newsockfd,"I got your message",18);

      if(n < 0)
        error("ERROR writing to socket");
      bzero(buffer,256);
>>>>>>> upstream/master
    }
    close(newsockfd);
  }
}

void ConnectionHandler::initializeConnection()
{
	std::cout << "[ConnectionHandler] Listening for connections" << std::endl;
	listen(mySocket,5);
	clientAddressLength = sizeof(clientAddress);
	outputSocket = accept(mySocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
	if (outputSocket < 0)
		error("ERROR on accept");
	bzero(buffer, bufferSize);
	setClientIP();
	std::cout << "[ConnectionHandler] Connection accepted: "
			  << clientIP << std::endl;
}

//assuming, that buffer is big enough for whole message
void ConnectionHandler::receiveMessage()
{
	int bytesRead = read(outputSocket, buffer, bufferSize);
	if (bytesRead < 0)
		error("ERROR reading from socket");
	std::cout << "[ConnectionHandler] Received command: " << buffer;

	commandQueue->push(new std::string(buffer));
	bzero(buffer, bufferSize);
	//sth with pointer, deserialization maybe?
}

void ConnectionHandler::sendResults()
{
	std::string *resultFilePath = resultFileQueue->pop();
	std::ifstream resultFile;
	resultFile.open(resultFilePath->data());
	while (true)
	{
		int bytesRead = readFileToBuffer(resultFile);
		if (bytesRead == 0)
				break;
	    writeBufferToOutputSocket(bytesRead);
	}
	deleteResultFile(resultFilePath);
}

int ConnectionHandler::readFileToBuffer(std::ifstream &resultFile)
{
	resultFile.read(buffer, bufferSize);
	int bytesRead = resultFile.gcount();
	if (bytesRead < 0)
		error("ERROR reading from file");
	return bytesRead;
}

void ConnectionHandler::writeBufferToOutputSocket(int bytesRead)
{
	void *bufferPointer = buffer;
	while (bytesRead > 0)
	{
		int bytesWritten = write(outputSocket, bufferPointer, bytesRead);
		if (bytesWritten <= 0)
			error("ERROR writing to socket");
		bytesRead -= bytesWritten;
		bufferPointer += bytesWritten;
	}
}

void ConnectionHandler::deleteResultFile(std::string * resultFilePath)
{
	if(remove(resultFilePath->data()) != 0)
		error("ERROR deleting file");
	delete resultFilePath;
}

void ConnectionHandler::closeConnection()
{
	close(outputSocket);
	std::cout << "[ConnectionHandler] Connection closed: "
			  << clientIP << std::endl << std::endl;
}

void ConnectionHandler::setClientIP()
{
	int part1 = clientAddress.sin_addr.s_addr&0xFF;
	int part2 = (clientAddress.sin_addr.s_addr&0xFF00)>>8;
	int part3 = (clientAddress.sin_addr.s_addr&0xFF0000)>>16;
	int part4 = (clientAddress.sin_addr.s_addr&0xFF000000)>>24;
	std::ostringstream stringStream;
	stringStream << part1 << "." << part2 << "." << part3 << "." << part4;
	clientIP = stringStream.str();
}

void ConnectionHandler::error(const char* message)
{
  std::cerr << message << std::endl; 
  exit(1);
}
