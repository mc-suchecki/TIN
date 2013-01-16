/*
 * ConnectionHandler.cpp
 *
 *  Created on: 26-12-2012
 *      Author: przemek
 */

#include "ConnectionHandler.hpp"
#include "../common/MessageDictionary.hpp"
#include <fstream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

ConnectionHandler::ConnectionHandler(int port, BlockingQueue<string> * commandQueue, ClientAuthenticator *clientAuth)
{
	this->commandQueue = commandQueue;
	clientAuthenticator = clientAuth;
	portNumber = port;

	initializeMySocket();
}

void ConnectionHandler::initializeMySocket()
{
	mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mySocket < 0)
		fatalError("ERROR opening socket");
	bzero((char *) &serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portNumber);
	if (bind(mySocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
		fatalError("ERROR on binding");
}

ConnectionHandler::~ConnectionHandler()
{
	close(mySocket);
}

void ConnectionHandler::start()
{
	connectionHandler = boost::thread(&ConnectionHandler::watchForConnections, this);
}

void ConnectionHandler::join()
{
	connectionHandler.join();
}

//i assume, that only one command is received before it's results are sent back
void ConnectionHandler::watchForConnections()
{
	while(true)
	{
		initializeConnection();
		if(isClientVerified())
		{
			receiveCommands();
		}
		closeConnection();
	}
}

void ConnectionHandler::initializeConnection()
{
	log("Listening for connections");
	listen(mySocket,5);
	clientAddressLength = sizeof(clientAddress);
	outputSocket = accept(mySocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
	if (outputSocket < 0)
		error("ERROR on accept");
	bzero(buffer, bufferSize);
	setClientIP();
	connectionOpened = true;
	log("Connection accepted: " + clientIP);
}

bool ConnectionHandler::isClientVerified()
{
	bool requestReceived = receiveChallengeRequest();
	if(!requestReceived)
		return false;

	sendChallenge();
	bool returnValue = receiveAndVerifyPassword();
	return returnValue;
}

bool ConnectionHandler::receiveChallengeRequest()
{
	readFromOutputSocketToBuffer(bufferSize);

	if( isTheCommand(MessageDictionary::sendChallenge, (string)buffer) )
		return true;
	return false;
}

void ConnectionHandler::sendChallenge()
{
	string challenge = clientAuthenticator->getChallenge();
	writeToOutputSocket( (char*)challenge.c_str(), challenge.length() );
	log("Challenge sent");
}

bool ConnectionHandler::receiveAndVerifyPassword()
{
	readFromOutputSocketToBuffer(bufferSize);

	bool passwordCorrect = clientAuthenticator->verifyPassword( (string)buffer );
	if(passwordCorrect)
	{
		string message = MessageDictionary::passwordCorrect;
		writeToOutputSocket( (char*)message.c_str(), message.length() );
		log("Client verified");
		return true;
	}
	else
	{
		string message = MessageDictionary::passwordIncorrect;
		writeToOutputSocket( (char*)message.c_str(), message.length() );
		log("Client rejected (wrong password)");
		return false;
	}
}

//assuming that buffer is big enough for whole message
void ConnectionHandler::receiveCommands()
{
	while(connectionOpened)
	{
		readFromOutputSocketToBuffer(bufferSize);
		recogizeAndExecuteCommand();
	}
}

void ConnectionHandler::recogizeAndExecuteCommand()
{
	string command = buffer;
	bzero(buffer, bufferSize);

	if(isTheCommand(MessageDictionary::sendFile, command))
		sendFile(command);

	else if(isTheCommand(MessageDictionary::closeConnection, command))
	{
		log("Received request: close connection");
		connectionOpened = false;
	}
	else
	{
		log("Received command: " + command);
		executeSystemCommand(command);
	}
}

bool ConnectionHandler::isTheCommand(const string &searchedString, const string &command)
{
	if(command.find(searchedString, 0) != string::npos)
		return true;
	return false;
}

void ConnectionHandler::sendFile(const string &command)
{
	//TODO some security in case of wrong file name
	string filePath = getFilePathFrom(command);
	log("Received request: send file '" + filePath + "'");
	std::ifstream fileToSend;
	fileToSend.open(filePath.c_str());
	while (true)
	{
		int bytesRead = readFromFileToBuffer(fileToSend);
		if (bytesRead == 0)
			break;

		writeToOutputSocket(buffer, bytesRead);
	}
	log("Sent file: '" + filePath + "'");
	fileToSend.close();
}

string ConnectionHandler::getFilePathFrom(const string &command)
{
	int bytesToSkip = MessageDictionary::sendFile.size();
	string newCommand = command.data() + bytesToSkip;
	return newCommand;
}

void ConnectionHandler::executeSystemCommand(const string &command)
{
	commandQueue->push(new string(command));
}

int ConnectionHandler::readFromFileToBuffer(std::ifstream &resultFile)
{
	bzero(buffer, bufferSize);
	resultFile.read(buffer, bufferSize);
	int bytesRead = resultFile.gcount();
	if (bytesRead < 0)
		error("ERROR reading from file");
	return bytesRead;
}

void ConnectionHandler::closeConnection()
{
	close(outputSocket);
	log("Connection closed: " + clientIP + "\n");
}

void ConnectionHandler::setClientIP()
{
	int IP_part1 = clientAddress.sin_addr.s_addr&0xFF;
	int IP_part2 = (clientAddress.sin_addr.s_addr&0xFF00)>>8;
	int IP_part3 = (clientAddress.sin_addr.s_addr&0xFF0000)>>16;
	int IP_part4 = (clientAddress.sin_addr.s_addr&0xFF000000)>>24;
	std::ostringstream stringStream;
	stringStream << IP_part1 << "." << IP_part2 << "." << IP_part3 << "." << IP_part4;
	clientIP = stringStream.str();
}

void ConnectionHandler::readFromOutputSocketToBuffer(int numberOfBytes)
{
	bzero(buffer, bufferSize);
	int bytesRead = read(outputSocket, buffer, numberOfBytes);
	if (bytesRead < 0)
		error("ERROR reading from socket");
	if (bytesRead == 0)
		error("ERROR socket empty");
}

void ConnectionHandler::writeToOutputSocket(char *message, int bytesToWrite)
{
	void *bufferPointer = message;
	while (bytesToWrite > 0)
	{
		int bytesWritten = write(outputSocket, bufferPointer, bytesToWrite);
		if (bytesWritten <= 0)
			error("ERROR writing to socket");
		bytesToWrite -= bytesWritten;
		bufferPointer += bytesWritten;
	}
}

void ConnectionHandler::log(const string & message) const
{
	cout << "[ConnectionHandler] " << message << endl;
}

void ConnectionHandler::error(const char* message)
{
	cerr << "[ConnectionHandler] " << message << endl;
	closeConnection();
	connectionOpened = false;
	watchForConnections();
}

void ConnectionHandler::fatalError(const char* message)
{
	cerr << "[ConnectionHandler] " << message << endl;
	exit(1);
}
