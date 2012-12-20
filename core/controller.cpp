#include "../include/controller.hpp"
#include "../include/eventQueue.hpp"
#include "../include/console.hpp"
#include "../include/logger.hpp"

#include "../include/events/consoleEvent.hpp"

using namespace std;

/** Controller constructor - responsible for creating main objects of the app. */
Controller::Controller() {
  fillEventActionMap();
  eventQueue = new EventQueue();
  console = new Console(eventQueue);

  // instanciate a logger, writing to standard output
  logger = new Logger(cout);

  boost::thread consoleThread = boost::thread(&Console::run, console);
}

/** Method responsible for run constantly and process events. */
void Controller::run() {
  Event *recievedEvent;
  MethodPointer requestedAction;

  while(true) {
    if(!eventQueue->isEmpty()) {
      recievedEvent = eventQueue->pop();

      //log the reception of an event
      logger->logEvent(recievedEvent);
      
      if(eventActionMap.find(&typeid(*recievedEvent)) != eventActionMap.end()){ 
        requestedAction = eventActionMap[&typeid(*recievedEvent)];
        (this->*requestedAction)(recievedEvent);
      }
      delete recievedEvent;
    }
  }
}

/** Method responsible for filling map connecting Events to methods handling them. */
void Controller::fillEventActionMap() {

  CreateConnectionEvent createConnectionEvent("");
  eventActionMap.insert(std::make_pair(&typeid(createConnectionEvent),
                                       &Controller::createConnection));
  SendCommandEvent sendCommandEvent;
  eventActionMap.insert(std::make_pair(&typeid(sendCommandEvent),
                                       &Controller::sendCommand));
  CancelAllEvent cancelAllEvent;
  eventActionMap.insert(std::make_pair(&typeid(cancelAllEvent),
                                       &Controller::cancelAll));
}

/** Method responsible for creating new connection. */
void Controller::createConnection(Event *event) {
  CreateConnectionEvent *createConnectionEvent = dynamic_cast<CreateConnectionEvent *>(event);
  //TODO
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
