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
        //following code should never be executed - it exists just in case
        //TODO - remove this after the project is done
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
  SendCommandEvent sendCommandEvent;
  eventActionMap.insert(std::make_pair(&typeid(sendCommandEvent),
        &Controller::sendCommand));
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
  CommandSendingFailedEvent commandSendingFailedEvent();
  eventActionMap.insert(std::make_pair(&typeid(commandSendingFailedEvent),
        &Controller::logMessage));
  ActionDoneEvent actionDoneEvent();
  eventActionMap.insert(std::make_pair(&typeid(actionDoneEvent),
        &Controller::saveResults));
  ReceivingResultsFailureEvent receivingResultsFailureEvent();
  eventActionMap.insert(std::make_pair(&typeid(receivingResultsFailureEvent),
        &Controller::logMessage));
}

/** Method responsible for creating new connection. */
void Controller::createConnection(Event *event) {
  CreateConnectionEvent *createConnectionEvent =
    dynamic_cast<CreateConnectionEvent *>(event);
  Connection *newConnection =
    new Connection(eventQueue, createConnectionEvent->getAddress(), 100);
  newConnection->init();
  activeConnections.push_back(newConnection);
  logger->logEvent(createConnectionEvent);
}

/** Method responsible for sending command to one of the servers using Connection. */
void Controller::sendCommand(Event *event) {
  SendCommandEvent *sendCommandEvent = dynamic_cast<SendCommandEvent *>(event);
  vector<Connection *>::iterator it;

  //find Connection with desired ip and send command
  for(it = activeConnections.begin(); it != activeConnections.end(); ++it) {
    if(sendCommandEvent->getAddress() == (*it)->getAddress()) {
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

/** Method responsible for cancelling all commands on one of the servers. */
void Controller::cancelAll(Event *event) {
  CancelAllEvent *cancelAllEvent = dynamic_cast<CancelAllEvent *>(event);
  vector<Connection *>::iterator it;

  //find Connection with desired ip and cancel his commands
  for(it = activeConnections.begin(); it != activeConnections.end(); ++it) {
    if(cancelAllEvent->getAddress() == (*it)->getAddress()) {
      (*it)->killAll();
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

/** Method used to handle ActionDoneEvents - it logs the event and saves the results. */
void Controller::saveResults(Event *event) {
  ActionDoneEvent *actionDoneEvent = dynamic_cast<ActionDoneEvent *>(event);
  //void *results = actionDoneEvent->getResults();
  //TODO save results to a file
  //TODO - how to delete results? "delete results" generates warning.
  logger->logEvent(actionDoneEvent);
}
