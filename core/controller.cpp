#include "../include/controller.hpp"
#include "../include/config.hpp"
#include "../include/eventQueue.hpp"
#include "../include/console.hpp"

#include "../include/events/consoleEvent.hpp"
#include "../include/events/connectionEvent.hpp"

#include <fstream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

using namespace std;

/** Controller constructor - responsible for creating main objects of the app. */
Controller::Controller(int argc, char * argv[]) {
  //handle configuration file nad commandline options
  config = Config::getInstance();
  if(!handleConfig(argc, argv)) exit(1);

  //initialize the application
  fillEventActionMap();
  eventQueue = new EventQueue();
  console = new Console(eventQueue);
  boost::thread consoleThread = boost::thread(&Console::run, console);

  //create appropriate logger instance
  if(config->getLogFile() == "no")
    logger = new ConsoleLogger(cout);
  else
    logger = new FileLogger();
}

/** Responsible for handling command line arguments and a config file */
bool Controller::handleConfig(int argc, char * argv[]){
  try {
    po::options_description genericOptions("Generic options");
    genericOptions.add_options()
      ("help", "help message")
      ("version", "version")
      ;
    po::options_description configOptions("Configuration");
    configOptions.add_options()
      ("port,p", po::value<int>(), "default port")
      ("file,f", po::value<string>(), "script file to run")
      ("config,c", po::value<string>(), "configuration file")
      ("debug,d", po::value<int>(), "debug level: 0 - none, 1 - events, 2 - all")
      ("logfile,l", po::value<string>(), "log file")
      ;

    po::options_description cmdLineOptions;
    cmdLineOptions.add(genericOptions).add(configOptions);

    po::options_description cfgFileOptions;
    cfgFileOptions.add(configOptions);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdLineOptions), vm);

    ifstream ifs("./settings.cfg"); // TODO add custom config files
    po::store(po::parse_config_file(ifs, cfgFileOptions), vm);

    po::notify(vm);

    if(vm.count("help")) {
      cout << cmdLineOptions << endl;
      return false;
    }

    if(vm.count("version")) {
      cout << "Version: 0.1"<< endl;
      return false;
    }

    if(vm.count("port"))
      config->setPort(vm["port"].as<int>());

    if(vm.count("debug"))
      config->setDebug(vm["debug"].as<int>());

    if(vm.count("logfile"))
      config->setLogFile(vm["logfile"].as<string>());

    if(vm.count("file"))
      config->setScriptFile(vm["file"].as<std::string>());

  } catch(exception& e) {
    cout << "error: " << e.what() << endl;
  } catch(...) {
    cout << "exception of unknown type" << endl;
  }

  return true;
}

/** Method responsible for running constantly and processing events. */
void Controller::run() {
  Event *receivedEvent;
  MethodPointer requestedAction;

  while(true) {
    if(!eventQueue->isEmpty()) {
      receivedEvent = eventQueue->pop();

      //find requested action for received event
      if(eventActionMap.find(&typeid(*receivedEvent)) != eventActionMap.end()) { 
        requestedAction = eventActionMap[&typeid(*receivedEvent)];
        (this->*requestedAction)(receivedEvent);
      } else {
        //following code should never be executed
        cout << "ERROR: Event handler not found for: "
          << typeid(*receivedEvent).name() << endl;
      }

      delete receivedEvent;
    }
  }
}

/** Method responsible for filling map connecting Events to methods handling them. */
void Controller::fillEventActionMap() {
  //console events
  CreateConnectionEvent createConnectionEvent;
  eventActionMap.insert(std::make_pair(&typeid(createConnectionEvent),
        &Controller::createConnection));

  CloseEvent closeEvent;
  eventActionMap.insert(std::make_pair(&typeid(closeEvent),
        &Controller::close));

  SendCommandEvent sendCommandEvent;
  eventActionMap.insert(std::make_pair(&typeid(sendCommandEvent),
        &Controller::sendCommand));

  GetFileEvent getFileEvent("","","");
  eventActionMap.insert(std::make_pair(&typeid(getFileEvent),
        &Controller::getFile));

  CancelAllEvent cancelAllEvent;
  eventActionMap.insert(std::make_pair(&typeid(cancelAllEvent),
        &Controller::cancelAll));

  //connection events
  ConnectionEstablishedEvent connectionEstablishedEvent;
  eventActionMap.insert(std::make_pair(&typeid(connectionEstablishedEvent),
        &Controller::logMessage));

  ConnectionFailedEvent connectionFailedEvent;
  eventActionMap.insert(std::make_pair(&typeid(connectionFailedEvent),
        &Controller::logMessage));

  CommandSentEvent commandSentEvent;
  eventActionMap.insert(std::make_pair(&typeid(commandSentEvent),
        &Controller::logMessage));

  CommandSendingFailedEvent commandSendingFailedEvent;
  eventActionMap.insert(std::make_pair(&typeid(commandSendingFailedEvent),
        &Controller::logMessage));

  ActionDoneEvent actionDoneEvent;
  eventActionMap.insert(std::make_pair(&typeid(actionDoneEvent),
        &Controller::logMessage));

  ReceivingResultsFailureEvent receivingResultsFailureEvent;
  eventActionMap.insert(std::make_pair(&typeid(receivingResultsFailureEvent),
        &Controller::logMessage));
}

/** Method responsible for creating new connection. */
void Controller::createConnection(Event *event) {
  CreateConnectionEvent *createConnectionEvent =
    dynamic_cast<CreateConnectionEvent *>(event);

  //acquire appropriate port
  int port = createConnectionEvent->getPort();
  if(port == 0) port = config->getPort();

  //create connection
  Connection *newConnection =
    new Connection(eventQueue, createConnectionEvent->getAddress(), port);
  newConnection->init(createConnectionEvent->getPassword()); 
  activeConnections.push_back(newConnection);

  logger->logEvent(createConnectionEvent);
}

void Controller::close(Event *event){
  CloseEvent *closeEvent =
    dynamic_cast<CloseEvent *>(event);

  vector<Connection *>::iterator it;

  //find Connection with desired IP and send command
  for(it = activeConnections.begin(); it != activeConnections.end(); ++it) {
    if(closeEvent->getAddress() == (*it)->getIPAddress()) {
      (*it)->close();
      logger->logEvent(closeEvent);
      return;
    }
  }
  logger->logEvent(closeEvent);
}

/** Method responsible for sending command to one of the servers using Connection. */
void Controller::sendCommand(Event *event) {
  SendCommandEvent *sendCommandEvent = dynamic_cast<SendCommandEvent *>(event);
  vector<Connection *>::iterator it;

  //find Connection with desired IP and send command
  for(it = activeConnections.begin(); it != activeConnections.end(); ++it) {
    if(sendCommandEvent->getAddress() == (*it)->getIPAddress()) {
      (*it)->execute(sendCommandEvent->getCommand());
      logger->logEvent(sendCommandEvent);
      return;
    }
  }

  //desired Connection not found - prompt user
  CommandSendingFailedEvent *errorEvent =
    new CommandSendingFailedEvent("Connection with IP: "+sendCommandEvent->getAddress()+" was not found!");
  logger->logEvent(errorEvent);
  delete errorEvent;
}

/** Method responsible for downloading the file */
void Controller::getFile(Event *event){
GetFileEvent *getFileEvent = dynamic_cast<GetFileEvent *>(event);
  vector<Connection *>::iterator it;

  //find Connection with desired IP and send command
  for(it = activeConnections.begin(); it != activeConnections.end(); ++it) {
    if(getFileEvent->getAddress() == (*it)->getIPAddress()) {
      (*it)->downloadFile(getFileEvent->getRemotePath(), getFileEvent->getLocalPath());
      logger->logEvent(getFileEvent);
      return;
    }
  }
}

/** Method responsible for cancelling all commands on one of the servers. */
void Controller::cancelAll(Event *event) {
  CancelAllEvent *cancelAllEvent = dynamic_cast<CancelAllEvent *>(event);
  vector<Connection *>::iterator it;

  //find Connection with desired IP and cancel his commands
  for(it = activeConnections.begin(); it != activeConnections.end(); ++it) {
    if(cancelAllEvent->getAddress() == (*it)->getIPAddress()) {
      (*it)->close(); //FIXME quickfix: killAll->close; killAll doesn't exist
      logger->logEvent(cancelAllEvent);
      return;
    }
  }

  //desired Connection not found - prompt user
  CommandSendingFailedEvent *errorEvent =
    new CommandSendingFailedEvent("Connection with this IP was not found!");
  logger->logEvent(errorEvent);
  delete errorEvent;
}

/** Method used to handle ConnectionEvents - it simply logs some message. */
void Controller::logMessage(Event *event) {
  ConnectionEvent *connectionEvent = dynamic_cast<ConnectionEvent *>(event);
  logger->logEvent(connectionEvent);
}
