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
    Command(const std::string &cmd): command(cmd) {}

    /// @return true if there is more data to be serialized (given chunk of data is not the last one)
    virtual unsigned int serialize(char *&serilizedChunk_out) const;
    virtual ~Command() {};

  protected:
    enum Type {
      COMMAND = 0x01,
      KILL_ALL = 0x02
    } type;

  private:
    const std::string command;
};

class Connection {
  public:
    typedef void (Connection::*MethodPtr)();
    Connection(EventQueue * const evQ, IPAddress addr, PortsNr p);
    virtual ~Connection();

    void init();
    void execute(const Command &command);
    void killAll();

    std::string getIPAddress();

  private:
    friend class InitAction;
    friend class KillAllAction;
    friend class ExecuteAction;
    class Action;

    boost::thread runThread;

    EventQueue * const eventQueue;
    BlockingQueue<Action> actionsQueue;
    volatile bool stopLoop;

    ////////////////////////////////////////
    const IPAddress IP_ADDRESS;
    const PortsNr PORTS_NUMBER;  
    boost::asio::ip::tcp::iostream remoteConnection;
    int sockfd;
    char buffer[BUFFER_SIZE];

    int numOfResults;

    void run();
    char* serialize(const std::string &command);

    //internal functions realizing tasks thread unsafely
    void init_internal();
    void execute_internal(const Command &command);
    void killAll_internal();

    //internal classes representing actions (design pattern: command)
    class Action {
      public:
        Action(Connection *conn): connection(conn){}
        virtual void execute() = 0;
        virtual ~Action() {};
      protected:
        Connection *connection;
    };

    class InitAction: public Action {
      public: 
        InitAction(Connection *conn): Action(conn) {};
        virtual ~InitAction() {};

        virtual void execute() {
          connection->init_internal();
        }
    };

    class ExecuteAction: public Action {
      public:
        ExecuteAction(Connection *conn, Command cmd):
          Action(conn), command(cmd) {};
        virtual ~ExecuteAction() {};

        virtual void execute() {
          connection->execute_internal(command);
        }

      private:
        Command command;
    };

    class KillAllAction: public Action {
      public: 
        KillAllAction(Connection *conn): Action(conn) {};
        virtual ~KillAllAction() {};

        virtual void execute() {
          connection->killAll_internal();
        }
    };
};

