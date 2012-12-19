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

/** Method responsible filling map connecting Events to methods handling them. */
void Controller::fillEventActionMap() {
  ConsoleEvent consoleEvent;
  eventActionMap.insert(std::make_pair(&typeid(consoleEvent), &Controller::handleConsoleEvent));
}

void Controller::handleConsoleEvent(Event *event) {
  ConsoleEvent *consoleEvent = dynamic_cast<ConsoleEvent *>(event);
  cout << "Recieved console event! Type is: " << typeid(*event).name() << endl;
  cout << "Receieved message is: " << consoleEvent->getMessage() << endl;
}
