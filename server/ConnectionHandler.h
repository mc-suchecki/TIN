#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <boost/thread.hpp>


class ConnectionHandler
{
    public:
        ConnectionHandler(int port);
        virtual ~ConnectionHandler();
        void start();
        void join();

    private:
        int sockfd, newsockfd, portno;
        socklen_t clilen;
        char buffer[256];
        struct sockaddr_in servAddr, cliAddr;

        //boost::thread communicationHandler;

        void startCommunication();
        void error(const char* message);
};

#endif // CONNECTIONHANDLER_H
