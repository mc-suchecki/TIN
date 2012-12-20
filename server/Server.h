#ifndef SERVER_H
#define SERVER_H
#include "ConnectionHandler.h"


class Server
{
    public:
        Server(int port);
        virtual ~Server();
    protected:
    private:
    ConnectionHandler *connectionHandler;
};

#endif // SERVER_H
