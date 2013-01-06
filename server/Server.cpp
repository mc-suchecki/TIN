/*
 * Server.cpp
 *
 *  Created on: 26-12-2012
 *      Author: przemek
 */

#include "Server.hpp"


Server::Server(int port)
{
	commandQueue = new BlockingQueue<std::string/*Command*/>;
	resultFileQueue = new BlockingQueue<std::string/*Command*/>;

	connectionHandler = new ConnectionHandler(port, commandQueue, resultFileQueue);
	connectionHandler->start();

	commandExecutor = new CommandExecutor(commandQueue, resultFileQueue);
	commandExecutor->start();
}

Server::~Server()
{
	connectionHandler->join();
	delete connectionHandler;

	commandExecutor->join();
	delete commandExecutor;

	delete commandQueue;
	delete resultFileQueue;
}
