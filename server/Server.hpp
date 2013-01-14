/*
 * Server.hpp
 *
 *  Created on: 26-12-2012
 *      Author: przemek
 */
#pragma once

#include "ConnectionHandler.hpp"
#include "CommandExecutor.hpp"
#include "blockingQueue.hpp"
#include <string>

class Server
{
	public:
		Server(int port);
		virtual ~Server();
	private:
		ConnectionHandler *connectionHandler;
		CommandExecutor * commandExecutor;
		BlockingQueue<std::string> * commandQueue;
		BlockingQueue<std::string> * resultFileQueue;
};

