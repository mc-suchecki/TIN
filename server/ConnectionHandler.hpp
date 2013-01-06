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
        ConnectionHandler(int port,
			  BlockingQueue<std::string/*Command*/> * commandQueue,
			  BlockingQueue<std::string> * resultFileQueue);
        virtual ~ConnectionHandler();
        void start();
        void join();

    private:
        char buffer[bufferSize];
        int mySocket, outputSocket, portNumber;
        socklen_t clientAddressLength;
        struct sockaddr_in serverAddress, clientAddress;
        std::string clientIP;

        boost::thread connectionHandler;

        BlockingQueue<std::string/*Command*/> * commandQueue;
        BlockingQueue<std::string> * resultFileQueue;

        void initializeMySocket();
        void watchForClientRequests();
        void initializeConnection();
        void receiveMessage();
        void sendResults();
        int readFileToBuffer(std::ifstream &resultFile);
        void writeBufferToOutputSocket(int bytesRead);
        void deleteResultFile(std::string * resultFilePath);
        void closeConnection();
        void setClientIP();
        void error(const char* message);
};
