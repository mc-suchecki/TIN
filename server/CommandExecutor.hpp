/*
 * CommandExecutor.hpp
 *
 *  Created on: 02-01-2013
 *      Author: przemek
 */

#pragma once

#include "blockingQueue.hpp"
#include <boost/thread.hpp>

class CommandExecutor
{
	public:
		CommandExecutor(BlockingQueue<std::string/*Command*/> * commandQueue,
				BlockingQueue<std::string> * resultFileQueue);
		virtual ~CommandExecutor() {}
		void start();
		void join();

	private:
		boost::thread commandExecutor;

		BlockingQueue<std::string/*Command*/> * commandQueue;
		BlockingQueue<std::string> * resultFileQueue;

		void watchForCommands();
		void execute(const std::string/*Command*/ *receivedCommand);
		std::string makeSystemCommand(const std::string *command, const std::string *filePath);
};

