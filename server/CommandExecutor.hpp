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
		CommandExecutor(BlockingQueue<std::string>*, BlockingQueue<std::string>*);
		virtual ~CommandExecutor() {}
		void start();
		void join();

	private:
		boost::thread commandExecutor;

		BlockingQueue<std::string> * commandQueue;
		BlockingQueue<std::string> * resultFileQueue;
		int resultFilesCounter;

		void watchForCommands();
		void execute(const std::string*);
		void sendResultFilesNumber();
		void beginNewExecutionSession();
		std::string* makeNewResultFileName();
		void executeSystemCommand(const std::string*, const std::string*);
		std::string makeSystemCommand(const std::string*, const std::string*);
};

