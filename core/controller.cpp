#include "../include/controller.hpp"
#include "../include/eventQueue.hpp"
#include "../include/console.hpp"

#include "../include/events/consoleEvent.hpp"
#include "../include/events/connectionEvent.hpp"

using namespace std;

/** Controller constructor - responsible for creating main objects of the app. */
Controller::Controller() {
  fillEventActionMap();
  eventQueue = new EventQueue();
  console = new Console(eventQueue);
  boost::thread consoleThread = boost::thread(&Console::run, console);
  logger = new Logger(cout);
}

/** Method responsible for run constantly and process events. */
void Controller::run() {
  Event *recievedEvent;
  MethodPointer requestedAction;

  while(true) {
    if(!eventQueue->isEmpty()) {
      recievedEvent = eventQueue->pop();
      logger->logEvent(recievedEvent);
      if(eventActionMap.find(&typeid(*recievedEvent)) != eventActionMap.end()) { 
        requestedAction = eventActionMap[&typeid(*recievedEvent)];
        (this->*requestedAction)(recievedEvent);
      }
      delete recievedEvent;
    }
  }
}

/** Method responsible for filling map connecting Events to methods handling them. */
void Controller::fillEventActionMap() {
  //console events
  //ConsoleEvent consoleEvent;
  //eventActionMap.insert(std::make_pair(&typeid(consoleEvent),
        //&Controller::handleConsoleEvent));
  CreateConnectionEvent createConnectionEvent("");
  eventActionMap.insert(std::make_pair(&typeid(createConnectionEvent),
        &Controller::createConnection));
  SendCommandEvent sendCommandEvent;
  eventActionMap.insert(std::make_pair(&typeid(sendCommandEvent),
        &Controller::sendCommand));
  CancelAllEvent cancelAllEvent;
  eventActionMap.insert(std::make_pair(&typeid(cancelAllEvent),
        &Controller::cancelAll));

  //connection events
  ConnectionEstablishedEvent connectionEstablishedEvent;
  eventActionMap.insert(std::make_pair(&typeid(cancelAllEvent),
        &Controller::displayMessage));
  ConnectionFailedEvent connectionFailedEvent;
  eventActionMap.insert(std::make_pair(&typeid(cancelAllEvent),
        &Controller::displayMessage));
  CommandSentEvent commandSentEvent;
  eventActionMap.insert(std::make_pair(&typeid(cancelAllEvent),
        &Controller::displayMessage));
  CommandSendingFailedEvent commandSendingFailedEvent();
  eventActionMap.insert(std::make_pair(&typeid(cancelAllEvent),
        &Controller::displayMessage));
  ActionDoneEvent actionDoneEvent();
  eventActionMap.insert(std::make_pair(&typeid(cancelAllEvent),
        &Controller::displayMessage));
  ReceivingResultsFailureEvent receivingResultsFailureEvent();
  eventActionMap.insert(std::make_pair(&typeid(cancelAllEvent),
        &Controller::displayMessage));
}

/** Method responsible for creating new connection. */
void Controller::createConnection(Event *event) {
  CreateConnectionEvent *createConnectionEvent =
    dynamic_cast<CreateConnectionEvent *>(event);
  Connection *newConnection =
    new Connection(eventQueue, createConnectionEvent->getAddress(), 100);
  newConnection->init();
  activeConnections.push_back(newConnection);
  cout << "Received CreateConnectionEvent with ip: " << createConnectionEvent->getAddress() << endl;
}

/** Method responsible for sending command to one of the servers using Connection. */
void Controller::sendCommand(Event *event) {
  SendCommandEvent *sendCommandEvent = dynamic_cast<SendCommandEvent *>(event);
  //TODO
}

/** Method responsible for cancelling all commands on one of the servers. */
void Controller::cancelAll(Event *event) {
  CancelAllEvent *cancelAllEvent = dynamic_cast<CancelAllEvent *>(event);
  //TODO
}

//TODO write comment
void Controller::displayMessage(Event *event) {
  ConnectionEvent *connectionEvent = dynamic_cast<ConnectionEvent *>(event);
  cout << "Recieved connection event! Type is: " << typeid(*event).name() << endl;
  cout << "Receieved message is: " << connectionEvent->getMessage() << endl;
}
