/*
 * Server.cpp
 *
 *  Created on: 26-12-2012
 *      Author: przemek
 */

#include "Server.hpp"

using std::string;

Server::Server(int port, string password)
{
	commandQueue = new BlockingQueue<string>;
	clientAuthenticator = new ClientAuthenticator(password);

	connectionHandler = new ConnectionHandler(port, commandQueue, clientAuthenticator);
	connectionHandler->start();

	commandExecutor = new CommandExecutor(commandQueue);
	commandExecutor->start();
}

Server::~Server()
{
	connectionHandler->join();
	delete connectionHandler;

	commandExecutor->join();
	delete commandExecutor;

	delete commandQueue;
}
