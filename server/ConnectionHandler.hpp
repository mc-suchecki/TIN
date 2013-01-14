/*
 * ConnectionHandler.hpp
 *
 *  Created on: 26-12-2012
 *      Author: przemek
 */

#pragma once

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <boost/thread.hpp>
#include "blockingQueue.hpp"

#define bufferSize 512

class ConnectionHandler
{
    public:
        ConnectionHandler(int port, BlockingQueue<std::string>*, BlockingQueue<std::string>*);
        virtual ~ConnectionHandler();
        void start();
        void join();

    private:
        static const std::string password;
        char buffer[bufferSize];
        int mySocket, outputSocket, portNumber;
        socklen_t clientAddressLength;
        struct sockaddr_in serverAddress, clientAddress;
        std::string clientIP;

        boost::thread connectionHandler;

        BlockingQueue<std::string> * commandQueue;
        BlockingQueue<std::string> * resultFileQueue;

        void initializeMySocket();
        void watchForClientRequests();
        void initializeConnection();
        void receiveCommands();
        void sendResultFiles();
        void closeConnection();

        bool isPasswordCorrect();
        bool verifyPasswordAndAnswer(char*);
        int sendNumberOfResultFiles();
        void waitForCommand(const std::string&);
        void sendFile(std::string*);
        int readFileToBuffer(std::ifstream&);
        void writeBufferToOutputSocket(int);
        void deleteResultFile(std::string*);
        void setClientIP();
        void error(const char*);
	void fatalError(const char*);
};
