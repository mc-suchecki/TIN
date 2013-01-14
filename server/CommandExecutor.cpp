/*
 * CommandExecutor.cpp
 *
 *  Created on: 02-01-2013
 *      Author: przemek
 */

#include "CommandExecutor.hpp"
#include "../common/MessageDictionary.hpp"
#include <fstream>

CommandExecutor::CommandExecutor(BlockingQueue<std::string/*Command*/> * commandQueue,
		  	  	  	  	  	  	 BlockingQueue<std::string> * resultFileQueue)
{
	this->commandQueue = commandQueue;
	this->resultFileQueue = resultFileQueue;
	resultFilesCounter = 0;
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

void CommandExecutor::execute(const std::string *receivedCommand)
{
	if( strcmp(receivedCommand->data(), MessageDictionary::sendResultFilesNumber.data()) == 0)
		sendResultFilesNumber();
	else if( strcmp(receivedCommand->data(), MessageDictionary::closeConnection.data()) == 0)
		beginNewExecutionSession();
	else
	{
		std::string *resultFilePath = makeNewResultFileName();
		executeSystemCommand(receivedCommand, resultFilePath);
		resultFileQueue->push(resultFilePath);
		resultFilesCounter++;
	}
	delete receivedCommand;
}

void CommandExecutor::sendResultFilesNumber()
{
	std::ostringstream stringStream;
	stringStream << resultFilesCounter;
	std::string *filesNumber = new std::string(stringStream.str());
	resultFileQueue->push(filesNumber);
}

void CommandExecutor::beginNewExecutionSession()
{
	resultFilesCounter = 0;
}

std::string* CommandExecutor::makeNewResultFileName()
{
	std::ostringstream stringStream;
	stringStream << "resultFile" << (resultFilesCounter+1);
	std::string *fileName = new std::string(stringStream.str());
	return fileName;
}

void CommandExecutor::executeSystemCommand(const std::string *systemCommand, const std::string *resultFilePath)
{
	std::cout << "[CommandExecutor] Executing command: " << systemCommand->data() << std::endl;
	std::string command = makeSystemCommand(systemCommand, resultFilePath);

	system(command.data());

	std::cout << "[CommandExecutor] Command executed: " << systemCommand->data() << std::endl;
}

std::string CommandExecutor::makeSystemCommand(const std::string *command, const std::string *filePath)
{
	std::string str1 = *command;
	str1.erase(std::remove(str1.begin(), str1.end(), '\n'), str1.end());
	std::string str2 = *filePath;
	str2.erase(std::remove(str2.begin(), str2.end(), '\n'), str2.end());

	return (str1 + " > " + str2 + " 2>&1");	//line ending redirects error output to standard output
}

