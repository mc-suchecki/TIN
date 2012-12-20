#include "ConnectionHandler.h"
//#include <boost/thread.hpp>

ConnectionHandler::ConnectionHandler(int port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &servAddr, sizeof(servAddr));

    portno = port;

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        error("ERROR on binding");
}

ConnectionHandler::~ConnectionHandler()
{
    close(sockfd);
}

void ConnectionHandler::start()
{
    startCommunication();
    //communicationHandler = boost::thread(&ConnectionHandler::StartCommunication, this);
}

void ConnectionHandler::join()
{
    //communicationHandler.join();
}

void ConnectionHandler::startCommunication()
{
    int n;
    while(true)
    {
        listen(sockfd,5);
        clilen = sizeof(cliAddr);
        newsockfd = accept(sockfd,
                 (struct sockaddr *) &cliAddr,
                 &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        bzero(buffer,256);
        while(n = read(newsockfd,buffer,255))
        {
            if (n < 0)
                error("ERROR reading from socket");
            printf("Here is the message: %s\n",buffer);
            n = write(newsockfd,"I got your message",18);
            if (n < 0)
                error("ERROR writing to socket");
            bzero(buffer,256);
        }
        close(newsockfd);
    }
}

void ConnectionHandler::error(const char* message)
{
    perror(message);
    exit(1);
}
