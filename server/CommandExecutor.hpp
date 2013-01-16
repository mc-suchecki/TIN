/*
 * CommandExecutor.hpp
 *
 *  Created on: 02-01-2013
 *      Author: przemek
 */

#pragma once

#include "blockingQueue.hpp"
#include <boost/thread.hpp>

using std::string;

class CommandExecutor
{
	public:
		CommandExecutor(BlockingQueue<string>*);
		virtual ~CommandExecutor() {}
		void start();
		void join();

	private:
		boost::thread commandExecutor;

		BlockingQueue<string> * commandQueue;

		void watchForCommands();
		void execute(const string*);
		void log(const string &) const;
};

