/*
 * CommandExecutor.cpp
 *
 *  Created on: 02-01-2013
 *      Author: przemek
 */

#include "CommandExecutor.hpp"
#include "../common/MessageDictionary.hpp"
#include <fstream>

using std::cout;
using std::endl;
using std::string;

CommandExecutor::CommandExecutor(BlockingQueue<std::string> * commandQueue)
{
	this->commandQueue = commandQueue;
}

void CommandExecutor::start()
{
	commandExecutor = boost::thread(&CommandExecutor::watchForCommands, this);
}

void CommandExecutor::join()
{
	commandExecutor.join();
}

void CommandExecutor::watchForCommands()
{
	while(true)
		execute(commandQueue->pop());
}

void CommandExecutor::execute(const string *receivedCommand)
{
	log("Executing command: " + *receivedCommand);
	string command = *receivedCommand;

	system(command.data());

	log("Command executed: " + *receivedCommand);
	delete receivedCommand;
}

void CommandExecutor::log(const string & message) const
{
	cout << "[CommandExecutor] " << message << endl;
}
