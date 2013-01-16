/*
 * Server.hpp
 *
 *  Created on: 26-12-2012
 *      Author: przemek
 */
#pragma once

#include "ClientAuthenticator.hpp"
#include "ConnectionHandler.hpp"
#include "CommandExecutor.hpp"
#include "blockingQueue.hpp"
#include <string>

class Server
{
	public:
		Server(int port, std::string password);
		virtual ~Server();
	private:
		ClientAuthenticator *clientAuthenticator;
		ConnectionHandler *connectionHandler;
		CommandExecutor * commandExecutor;
		BlockingQueue<std::string> * commandQueue;
};

