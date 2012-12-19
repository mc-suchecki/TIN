#include "../include/controller.hpp"
#include "../include/eventQueue.hpp"
#include "../include/console.hpp"

using namespace std;

/** Controller constructor - responsible for creating main objects of the app. */
Controller::Controller() {
  //creating objects
  eventQueue = new EventQueue();
  console = new Console(eventQueue);
}

/** Method responsible for run constantly and process events. */
void Controller::run() {
  while(true) {
    //<delete>
    console->run();
    //</delete>
  }
}
