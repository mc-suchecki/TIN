#include "Server.h"


Server::Server(int port)
{
    connectionHandler = new ConnectionHandler(port);
    connectionHandler->start();
}

Server::~Server()
{
    connectionHandler->join();
    delete connectionHandler;
}
