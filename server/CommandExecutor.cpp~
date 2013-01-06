/*
 * CommandExecutor.cpp
 *
 *  Created on: 02-01-2013
 *      Author: przemek
 */

#include "CommandExecutor.hpp"
#include <fstream>

CommandExecutor::CommandExecutor(BlockingQueue<std::string/*Command*/> * commandQueue,
				 BlockingQueue<std::string> * resultFileQueue)
{
	this->commandQueue = commandQueue;
	this->resultFileQueue = resultFileQueue;
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
	{
		execute(commandQueue->pop());
	}
}

void CommandExecutor::execute(const std::string/*Command*/ *receivedCommand)
{
	std::cout << "[CommandExecutor] Executing command: " << receivedCommand->data();
	std::string *resultFilePath = new std::string("testResultFile");
	std::string command = makeSystemCommand(receivedCommand, resultFilePath);

	system(command.data());

	std::cout << "[CommandExecutor] Command executed: " << receivedCommand->data();
	delete receivedCommand;
	resultFileQueue->push(resultFilePath);
}

std::string CommandExecutor::makeSystemCommand(const std::string *command, const std::string *filePath)
{
	std::string str1 = *command;
	str1.erase(std::remove(str1.begin(), str1.end(), '\n'), str1.end());
	std::string str2 = *filePath;
	str2.erase(std::remove(str2.begin(), str2.end(), '\n'), str2.end());

	return (str1 + " > " + str2 + " 2>&1");	//line ending redirects error output to standard output
}

