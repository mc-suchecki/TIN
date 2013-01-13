/*
 * ConnectionHandler.cpp
 *
 *  Created on: 26-12-2012
 *      Author: przemek
 */

#include "ConnectionHandler.hpp"
#include "../common/MessageDictionary.hpp"
#include <fstream>

const std::string ConnectionHandler::password = "password";

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
	mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mySocket < 0)
		error("ERROR opening socket");
	bzero((char *) &serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portNumber);
	if (bind(mySocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
		error("ERROR on binding");
}

ConnectionHandler::~ConnectionHandler()
{
	close(mySocket);
}

void ConnectionHandler::start()
{
	connectionHandler = boost::thread(&ConnectionHandler::watchForClientRequests, this);
}

void ConnectionHandler::join()
{
	connectionHandler.join();
}

//i assume, that only one command is received before it's results are sent back
void ConnectionHandler::watchForClientRequests()
{
	while(true)
	{
		initializeConnection();
		if(isPasswordCorrect())
		{
			receiveCommands();
			sendResultFiles();
		}
		closeConnection();
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

bool ConnectionHandler::isPasswordCorrect()
{
	int bytesRead = read(outputSocket, buffer, bufferSize);
	if (bytesRead < 0)
		error("ERROR reading from socket");
	bool returnValue = verifyPasswordAndAnswer(buffer);
	bzero(buffer, bufferSize);
	return returnValue;
}

bool ConnectionHandler::verifyPasswordAndAnswer(char* readPassword)
{
	if( !strcmp(readPassword, password.data()) )
	{
		int bytesWritten = write(outputSocket, (void*)MessageDictionary::passwordIncorrect.data(), bufferSize);
		if (bytesWritten <= 0)
			error("ERROR writing to socket");
		std::cout << "[ConnectionHandler] Unknown client rejected" << std::endl;
		return false;
	}
	else
	{
		int bytesWritten = write(outputSocket, (void*)MessageDictionary::passwordIncorrect.data(), bufferSize);
		if (bytesWritten <= 0)
			error("ERROR writing to socket");
		std::cout << "[ConnectionHandler] Client verified" << std::endl;
		return true;
	}
}

//assuming, that buffer is big enough for whole message
void ConnectionHandler::receiveCommands()
{
	while(true)
	{
		int bytesRead = read(outputSocket, buffer, bufferSize);
		if (bytesRead < 0)
			error("ERROR reading from socket");

		std::cout << "[ConnectionHandler] Received command: " << buffer;
		commandQueue->push(new std::string(buffer));
		if( strcmp(buffer, MessageDictionary::sendResultFilesNumber.data()) )
		{
			bzero(buffer, bufferSize);
			break;
		}
		bzero(buffer, bufferSize);
	}
}

void ConnectionHandler::sendResultFiles()
{
	int numberOfFiles = sendNumberOfResultFiles();
	waitForCommand(MessageDictionary::sendResultFiles);
	for(int i=0 ; i < numberOfFiles ; i++)
		sendFile(resultFileQueue->pop());
	std::cout << "[ConnectionHandler] Sent " << numberOfFiles << " result files" << std::endl;
}

int ConnectionHandler::sendNumberOfResultFiles()
{
	std::string *numberOfResultFiles = resultFileQueue->pop();
	int numberOfFiles = atoi(numberOfResultFiles->data());
	int bytesWritten = write(outputSocket, (void*)numberOfResultFiles->data(), bufferSize);
	if (bytesWritten <= 0)
		error("ERROR writing to socket");

	delete numberOfResultFiles;
	return numberOfFiles;
}

void ConnectionHandler::waitForCommand(const std::string &command)
{
	while(true)
	{
		int bytesRead = read(outputSocket, buffer, bufferSize);
		if (bytesRead < 0)
			error("ERROR reading from socket");

		if( strcmp(buffer, command.data()) )
		{
			std::cout << "[ConnectionHandler] Received command: " << buffer;
			commandQueue->push(new std::string(buffer));
			bzero(buffer, bufferSize);
			break;
		}
		bzero(buffer, bufferSize);
	}
}

void ConnectionHandler::sendFile(std::string *filePath)
{
	std::ifstream resultFile;
	resultFile.open(filePath->data());
	while (true)
	{
		int bytesRead = readFileToBuffer(resultFile);
		if (bytesRead == 0)
			break;
		writeBufferToOutputSocket(bytesRead);
	}
	deleteResultFile(filePath);
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
	waitForCommand(MessageDictionary::closeConnection);
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
	perror(message);
	exit(1);
}
