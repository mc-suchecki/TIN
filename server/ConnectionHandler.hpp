/*
 * ConnectionHandler.hpp
 *
 *  Created on: 26-12-2012
 *      Author: przemek
 */

#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <boost/thread.hpp>
#include "blockingQueue.hpp"
#include "ClientAuthenticator.hpp"

#define bufferSize 2048

using std::string;

class ConnectionHandler
{
    public:
        ConnectionHandler(int port, BlockingQueue<string>*, ClientAuthenticator*);
        virtual ~ConnectionHandler();
        void start();
        void join();

    private:
        char buffer[bufferSize];
        int mySocket, outputSocket, portNumber;
        socklen_t clientAddressLength;
        struct sockaddr_in serverAddress, clientAddress;
        string clientIP;
        bool connectionOpened;

        boost::thread connectionHandler;
        ClientAuthenticator * clientAuthenticator;
        BlockingQueue<string> * commandQueue;

        void initializeMySocket();
        void watchForConnections();
        void initializeConnection();
        void receiveCommands();
        void closeConnection();

        bool isClientVerified();
        bool receiveChallengeRequest();
        void sendChallenge();
        bool receiveAndVerifyPassword();
        void recogizeAndExecuteCommand();
        bool isTheCommand(const string&, const string&);
        void sendFile(const string&);
        void sendFileSize(uint);
        std::ifstream::pos_type filesize(const char*);
        string getFilePathFrom(const string&);
        void executeSystemCommand(const string&);
        int readFromFileToBuffer(std::ifstream&);

        void readFromOutputSocketToBuffer(int);
        void writeToOutputSocket(char*, int);
        void setClientIP();
        void log(const string &) const;
        void error(const char*);
        void fatalError(const char*);
};
