#include "../include/controller.hpp"
#include "../include/eventQueue.hpp"
#include "../include/console.hpp"

#include "../include/events/consoleEvent.hpp"

using namespace std;

/** Controller constructor - responsible for creating main objects of the app. */
Controller::Controller() {
  fillEventActionMap();
  eventQueue = new EventQueue();
  console = new Console(eventQueue);
  boost::thread consoleThread = boost::thread(&Console::run, console);
}

/** Method responsible for run constantly and process events. */
void Controller::run() {
  Event *recievedEvent;
  MethodPointer requestedAction;

  while(true) {
    if(!eventQueue->isEmpty()) {
      recievedEvent = eventQueue->pop();
      requestedAction = eventActionMap[&typeid(*recievedEvent)];
      (this->*requestedAction)(recievedEvent);
      delete recievedEvent;
    }
  }
}

/** Method responsible for filling map connecting Events to methods handling them. */
void Controller::fillEventActionMap() {
  ConsoleEvent consoleEvent;
  eventActionMap.insert(std::make_pair(&typeid(consoleEvent),
                                       &Controller::handleConsoleEvent));
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
  cout << "Received CreateConnection event with address: "
   << createConnectionEvent->getAddress()
   << endl;
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

//TODO - remove me!
void Controller::handleConsoleEvent(Event *event) {
  ConsoleEvent *consoleEvent = dynamic_cast<ConsoleEvent *>(event);
  cout << "Recieved console event! Type is: " << typeid(*event).name() << endl;
  cout << "Receieved message is: " << consoleEvent->getMessage() << endl;
}
