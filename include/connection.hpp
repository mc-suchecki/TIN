/** @author Jacek Witkowski
 *
 *  Definition of class representing connection with server. It is used
 *  by clients application.
 */

#pragma once

#include <string>
#include <map>
#include <boost/asio.hpp>

#include "eventQueue.hpp"
#include "blockingQueue.hpp"

// Maybe I will make separate class for IPAddress and PortsNr in the future
typedef std::string IPAddress;
typedef unsigned short PortsNr;

/**
 * Class representing the connection with a remote server. Used for sending
 * commands and receiving results of their execution. Every instance of that
 * class represents a separate Server.
 */
const unsigned int BUFFER_SIZE = 256;

class Command {
  public:
    enum Type {
      COMMAND = 0x01,
      CLOSE = 0x02
    };

    Command(const std::string &cmd, enum Type t=Command::COMMAND):
      type(t), command(cmd) {}

    virtual unsigned int serialize(char *&serilizedChunk_out) const;
    virtual ~Command() {};

  protected:
    Type type;

  private:
    const std::string command;
};

class Connection {
  public:
    typedef void (Connection::*MethodPtr)();
    Connection(EventQueue * const evQ, IPAddress addr, PortsNr p);
    virtual ~Connection();

    void init(std::string password);
    void execute(const Command &command);
    void close();
    void downloadFile(std::string remoteFile, std::string localFile);

    std::string getIPAddress();

  private:
    friend class InitAction;
    friend class CloseAction;
    friend class ExecuteAction;
    friend class DownloadFileAction;
    class Action;

    boost::thread runThread;

    EventQueue * const eventQueue;
    BlockingQueue<Action> actionsQueue;
    volatile bool stopLoop;

    ////////////////////////////////////////
    const IPAddress IP_ADDRESS;
    const PortsNr PORTS_NUMBER;  
    boost::asio::ip::tcp::iostream remoteConnection;
    volatile int sockfd;
    struct sockaddr_in servAddr;
    char buffer[BUFFER_SIZE];

    void run();
    char* serialize(const std::string &command);

    //internal functions realizing tasks thread unsafely
    void init_internal(std::string password);
    void execute_internal(const Command &command);
    void close_internal();
    void downloadFile_internal(std::string remoteFile, std::string localFile);

    bool prepareSocket();
    bool authenticate(std::string password);
    bool sendAuthenticationInfo(std::string password, std::string challenge);
    bool sendCommand(const Command &command);
    int receiveMsg();
    bool receiveAndSaveFile(std::string destFile);
    void getCurrTime(char *timeBuff, int n);
    std::string getChallenge();

    //internal classes representing actions (design pattern: command)
    class Action {
      public:
        Action(Connection *conn): connection(conn){}
        virtual void execute() = 0;
        virtual ~Action() {}
      protected:
        Connection *connection;
    };

    class InitAction: public Action {
      public: 
        InitAction(Connection *conn, std::string pwd): Action(conn), password(pwd) {}
        virtual ~InitAction() {}

        virtual void execute() {
          connection->init_internal(password);
        }

      private:
        std::string password;
    };

    class ExecuteAction: public Action {
      public:
        ExecuteAction(Connection *conn, Command cmd):
          Action(conn), command(cmd) {}
        virtual ~ExecuteAction() {}

        virtual void execute() {
          connection->execute_internal(command);
        }

      private:
        Command command;
    };

    class CloseAction: public Action {
      public: 
        CloseAction(Connection *conn): Action(conn) {}
        virtual ~CloseAction() {}

        virtual void execute() {
          connection->close_internal();
        }
    };

    class DownloadFileAction: public Action {
      public:
        DownloadFileAction(Connection *conn, std::string src, std::string dst):
          Action(conn), sourceFile(src), destinationFile(dst) {}
        virtual ~DownloadFileAction() {}

        virtual void execute() {
          connection->downloadFile_internal(sourceFile, destinationFile);
        }
      private:
        std::string sourceFile, destinationFile;
    };
};

